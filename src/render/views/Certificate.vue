<template>
  <div>
    <el-page-header @back="goBack" content="Certificate Tool" class="header" title></el-page-header>
    <el-row>
      <el-col :span="14">
        <div class="certTree" :style="{height:winHeight}">
          <el-tree
            :data="data"
            :props="defaultProps"
            @node-click="handleNodeClick"
            default-expand-all
            :expand-on-click-node="false"
          >
            <span class="custom-tree-node" slot-scope="{ node,data}">
              <span>
                <el-tag size="mini">{{ node.label }}</el-tag>
                <span class="node-content">{{data.content}}</span>
                <strong>{{data.info}}</strong>
              </span>
            </span>
          </el-tree>
        </div>
      </el-col>
      <el-col :span="10">
        <div style="width:100%;height:10px" />
        <div>
          <div style="margin:10px">
            <el-tag effect="dark">Offset:</el-tag>
            <span style="margin-left:10px">{{offset}}</span>
          </div>
          <div style="margin:10px">
            <el-tag effect="dark" type="success">Length:</el-tag>
            <span style="margin-left:10px">{{header}}+{{length}}</span>
          </div>
          <div class="upload">
            <el-upload
              drag
              action="http://localhost.com"
              :multiple="false"
              :auto-upload="false"
              :on-change="fileUpload"
              :on-remove="fileRemove"
              :file-list="fileList"
              accept=".pem"
            >
              <i class="el-icon-upload"></i>
              <div class="el-upload__text">Upload Certificate</div>
            </el-upload>
          </div>
          <!-- <codemirror
            v-model="code"
            ref="cmEditor"
            :options="cmOptions"
          /> -->
          <div v-if="fileList.length>0">
            <div style="margin:10px">
              <el-tag effect="dark" type="info">KeyAlgorithm:</el-tag>
              <span style="margin-left:10px">{{keyType}}</span>
            </div>
            <div style="margin:10px">
              <el-tag effect="dark" type="info">signAlgorithm:</el-tag>
              <span style="margin-left:10px">{{signWay}}</span>
            </div>
            <div style="margin:10px">
              <el-tag effect="dark" type="info">keyContainerLen:</el-tag>
              <span style="margin-left:10px">{{keyContainerLen}}</span>
            </div>
            <div style="margin:10px">
              <el-tag effect="dark" type="info">pKeyContainer:</el-tag>
              <span style="margin-left:10px">{{pKeyContainer}}</span>
            </div>
            <div style="margin:10px" v-for="(key,index) in signOffset" :key="key.offset">
              <el-tag effect="dark" type="info">signatureValue[{{index}}]:</el-tag>
              <span
                style="margin-left:10px"
              >pAuth[{{index}}]:{{key.offset}},authLen[{{index}}]:{{key.len}}</span>
            </div>

            <div style="margin:10px" v-for="(key,index) in keyoffset" :key="index">
              <el-tag effect="dark" type="info">keyValue[{{index}}]:</el-tag>
              <span
                style="margin-left:10px"
              >pKey[{{index}}]:{{key.offset}},keyLen[{{index}}]:{{key.len}}</span>
            </div>
            <div>
              <el-button
                type="text"
                icon="el-icon-document-copy"
                class="copy-btn1"
                :data-clipboard-text="der"
              ></el-button>
              <el-input
                type="textarea"
                placeholder="DER"
                :autosize="{ minRows: 2, maxRows: 15}"
                v-model="der"
                readonly
              ></el-input>
            </div>
          </div>
        </div>
      </el-col>
    </el-row>
  </div>
</template>
<script>
import ASN1 from "@lapo/asn1js";
// import Hex from "@lapo/asn1js/hex";
import oids from "@lapo/asn1js/oids";
import Base64 from "./../components/base64.js";
const ClipboardJS = require("clipboard/dist/clipboard.js");
// const { pki } = require('node-forge')
const { ipcRenderer } = require("electron");
var lineLength = 50;

export default {
  data() {
    return {
      data: [],
      defaultProps: {
        children: "children",
        label: "label",
      },
      der: "",
      offset: 0,
      header: 0,
      length: 0,
      fileList: [],
      winHeight: "",
      keyType: "",
      signWay: "",
      signOffset: [],
      keyoffset: [],
      clipboard: {},
      keyContainerLen: 0,
      pKeyContainer: "",
    };
  },
  mounted() {
    this.clipboard = new ClipboardJS(".copy-btn1");
    this.clipboard.on("success", () => {
      this.$notify({
        title: "成功",
        message: "复制成功",
        type: "success",
      });
    });
    window.addEventListener("resize", this.resizeTerminal);
    this.winHeight = window.innerHeight - 200 + "px";
  },
  destroyed() {
    this.clipboard.destroy();
    window.removeEventListener("resize", this.resizeTerminal);
  },
  methods: {
    resizeTerminal() {
      this.winHeight = window.innerHeight - 200 + "px";
    },
    goBack() {
      this.$router.push("/");
    },
    handleNodeClick(e) {
      this.header = e.header;
      this.length = e.length;
      this.offset = e.offset;
    },
    fileRemove(file, fileList) {
      if (fileList.length == 0) {
        this.data = [];
      }
    },
    fileUpload(file, fileList) {
      var name=file.name.replace('.','_')
      console.log(file)
      this.keyoffset = [];
      this.signOffset = [];
      this.fileList = fileList.slice(-1);
      var der = ipcRenderer.sendSync("readCertDer", file.raw.path);
      if (Base64.re.test(der)) {
        var hexArray = Base64.unarmor(der);
        var asn1 = ASN1.decode(hexArray);
        this.data = this.toTree(asn1);
        var bytes = [];
        for (var c = 0; c < hexArray.length; c++) {
          bytes.push("0x" + hexArray[c].toString(16));
        }
        this.der = `static const uint8_t ${name}_key[${
          bytes.length
        }]=\r{`;
        for(let i=0;i<bytes.length/16;i++){
          this.der+=`${bytes.slice(i*16,(i+1)*16).toString()},\r` 
        }
        this.der+='};\r'


        if (this.data[0].children[0].children[6].children[0].children[0]) {
          this.keyType = this.data[0].children[0].children[6].children[0].children[0].oid.d;
        }
        if (this.data[0].children[1].children[0]) {
          this.signWay = this.data[0].children[1].children[0].oid.d;
        }
        this.keyContainerLen = this.data[0].children[0].length + this.data[0].children[0].header
        this.pKeyContainer = `&derKey[${
          this.data[0].children[0].offset 
        }]`;

        if (this.keyType == "rsaEncryption") {
          let item = this.data[0].children[0].children[6].children[1]
            .children[0];
          if (item.children[0]) {
            this.keyoffset.push({
              offset: `&derKey[${
                item.children[0].offset + item.children[0].header + 1
              }]`,
              len: item.children[0].length - 1,
            });
          }
          if (item.children[1]) {
            this.keyoffset.push({
              offset: `&derKey[${
                item.children[1].offset + item.children[1].header
              }]`,
              len: item.children[1].length,
            });
          }
        } else if (this.keyType == "ecPublicKey") {
          let item = this.data[0].children[0].children[6].children[1];
          if (item) {
            this.keyoffset.push({
              offset: `&derKey[${
                item.offset + item.header + 2 //RFC5480-2.2  The first octet of the OCTET STRING indicates whether the key iscompressed or uncompressed.
              }]`,
              len: item.length - 2,
            });
          }
        } else if(this.keyType == "curveEd25519") {
          let item = this.data[0].children[0].children[6].children[1];
          if (item) {
            this.keyoffset.push({
              offset: `&derKey[${
                item.offset + item.header + 1 //RFC5480-2.2  The first octet of the OCTET STRING indicates whether the key iscompressed or uncompressed.
              }]`,
              len: item.length - 1,
            });
          }
        }

        if (this.signWay.indexOf("RSAEncryption") != -1) {
          if (this.data[0].children[2]) {
            this.signOffset.push({
              offset: `&derKey[${
                this.data[0].children[2].offset +
                this.data[0].children[2].header +
                1
              }]`,
              len: this.data[0].children[2].length - 1,
            });
          }
        } else if (this.signWay.indexOf("ecdsaWith") != -1) {
          /* rfc3279 2.2.3 
            Ecdsa-Sig-Value  ::=  SEQUENCE  {
                  r     INTEGER,
                  s     INTEGER  }
          */
          let item = this.data[0].children[2].children[0];
          if (item) {
            this.signOffset.push({
              offset: `&derKey[${
                item.children[0].offset + item.children[0].header + 1
              }]`,
              len: item.children[0].length - 1,
            });
            this.signOffset.push({
              offset: `&derKey[${
                item.children[1].offset + item.children[1].header + 1
              }]`,
              len: item.children[1].length - 1,
            });
          }
        } else if(this.signWay.indexOf("Ed25519") != -1) {
          if (this.data[0].children[2]) {
            this.signOffset.push({
              offset: `&derKey[${
                this.data[0].children[2].offset +
                this.data[0].children[2].header +
                1
              }]`,
              len: this.data[0].children[2].length - 1,
            });
          }
        }

        
      }
    },
    toTree(obj) {
      var isOID =
        typeof oids === "object" &&
        obj.tag.isUniversal() &&
        obj.tag.tagNumber == 0x06;
      var node = {};
      //   node.asn1 = obj;
      node.label = obj.typeName().replace(/_/g, " ");
      node.content = "";
      node.info = "";
      var content = obj.content();
      if (content !== null) {
        var shortContent;
        if (isOID) content = content.split("\n", 1)[0];
        shortContent =
          content.length > lineLength
            ? content.substring(0, lineLength) + "\u2026"
            : content;
        node.content += " " + shortContent;
        if (isOID) {
          var oid = oids[content];
          if (oid) {
            node.oid = oid;
            if (oid.d) {
              node.info += " " + oid.d;
            }
            if (oid.c) {
              node.info += " " + oid.c;
            }
          }
        }
      }
      node.offset = obj.stream.pos;
      node.header = obj.header;
      node.length = obj.length;

      var children = [];
      if (obj.sub !== null) {
        for (var i = 0, max = obj.sub.length; i < max; ++i)
          children.push(this.toTree(obj.sub[i])[0]);
      }
      node.children = children;
      return [node];
    },
  },
};
</script>
<style>
.custom-tree-node {
  flex: 1;
  display: flex;
  align-items: center;
  justify-content: space-between;
  font-size: 14px;
  padding-right: 8px;
}
.node-content {
  margin-left: 1em;
  color: #909399;
}
.copy-btn1 {
  position: absolute;
  z-index: 2;
  right: 30px;
}
.certTree {
  overflow: auto;
  margin-right: 20px;
}
.upload {
  text-align: center;
}
.cert {
  position: fixed;
  z-index: 20;
  top: 10px;
}
.keyPos {
  position: fixed;
  z-index: 20;
  top: 30px;
}
</style>