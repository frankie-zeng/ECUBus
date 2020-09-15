<template>
  <div>
    <el-page-header @back="goBack" content="Certificate Tool" class="header" title></el-page-header>
    <el-row>
      
      <el-col :span="10" >
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
      <el-col :span="6">
        <div style="width:100%;height:10px" />
        <div>
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
          <div v-if="fileList.length>0">
            <div style="margin:10px">
              <el-tag effect="dark">Offset:</el-tag>
              <span style="margin-left:10px">{{offset}}</span>
            </div>
            <div style="margin:10px">
              <el-tag effect="dark" type="success">Length:</el-tag>
              <span style="margin-left:10px">{{header}}+{{length}}</span>
            </div>

            <el-input
              type="textarea"
              placeholder="DER"
              :autosize="{ minRows: 2, maxRows: 20}"
              v-model="der"
              readonly
            ></el-input>
          </div>
        </div>
      </el-col>
      <el-col :span="8">
        <div >
          <el-tree
            :data="keyImport"
            :props="defaultProps"
            @node-click="keyImportClick"
            default-expand-all
            :expand-on-click-node="false"
          >
            <span class="custom-tree-node" slot-scope="{ node,data}">
              <span>
              {{ node.label }}
              </span>
              <span v-if="data.type=='input'">
                <el-input v-model="data.value" placeholder size="mini" class="keyinput"></el-input>
              </span>
            </span>
          </el-tree>
        </div>
      </el-col>
    </el-row>
  </div>
</template>
<script>
import ASN1 from "@lapo/asn1js";
import Hex from "@lapo/asn1js/hex";
import oids from "@lapo/asn1js/oids";
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
      winHeight:'',
      keyImport: [
        {
          label: "targetKeyHandle",
          value: "",
        },
        {
          label: "pKeyInfo",
          value: "",
        },
        {
          label: "pKey",
          children: [
            {
              type:'input',
              value: "",
              label: "pKey[0]",
            },
            {
              type:'input',
              value: "",
              label: "pKey[1]",
            },
            {
              type:'input',
              value: "",
              label: "pKey[2]",
            },
          ],
        },
        {
          label: "keyLen",
          value: "",
          children: [
            {
              value: "",
              label: "keyLen[0]",
              type:'input',
            },
            {
              value: "",
              label: "keyLen[1]",
              type:'input',
            },
            {
              value: "",
              label: "keyLen[2]",
              type:'input',
            },
          ],
        },
        {
          label: "cipher",
          value: "",
          children: [
            {
              value: "",
              label: "cipherKeyHandle",
            },
            {
              value: "",
              label: "cipherScheme",
            },
          ],
        },
        {
          label: "keyContainer",
          value: "",
          children: [
            {
              value: "",
              label: "keyContainerLen",
            },
            {
              value: "",
              label: "pKeyContainer",
            },
            {
              value: "",
              label: "authKeyHandle",
            },
            {
              value: "",
              label: "authScheme",
            },
            {
              value: "",
              label: "authLen",
              children: [
                {
                  value: "",
                  label: "authLen[0]",
                },
                {
                  value: "",
                  label: "authLen[1]",
                },
              ],
            },
            {
              label: "pAuth",
              value: "",
              children: [
                {
                  value: "",
                  label: "pAuth[0]",
                },
                {
                  value: "",
                  label: "pAuth[1]",
                },
              ],
            },
          ],
        },
      ],
    };
  },
  mounted() {
    window.addEventListener("resize", this.resizeTerminal);
    this.winHeight=window.innerHeight-200+'px'
  },
  destroyed() {
    window.removeEventListener("resize",this.resizeTerminal);
  },
  methods: {
    resizeTerminal(){
      this.winHeight=window.innerHeight-200+'px'
    },
    goBack() {
      this.$router.push("/");
    },
    handleNodeClick(e) {
      this.header = e.header;
      this.length = e.length;
      this.offset = e.offset;
    },
    keyImportClick(e) {
      console.log(e);
    },
    fileRemove(file, fileList) {
      if (fileList.length == 0) {
        this.data = [];
      }
    },
    fileUpload(file, fileList) {
      console.log(file);
      this.fileList = fileList.slice(-1);
      var der = ipcRenderer.sendSync("readCertDer", file.raw.path);
      var asn1 = ASN1.decode(Hex.decode(der));
      this.data = this.toTree(asn1);
      var bytes = [];
      for (var c = 0; c < der.length; c += 2) {
        bytes.push(parseInt(der.substr(c, 2), 16));
      }

      this.der = `uint8_t derKey[${bytes.length}]=\r{${bytes.toString()}};`;
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
.certTree{
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
.keyPos{
  position: fixed;
  z-index: 20;
  top: 30px
}
.el-tree-node__content{
  height: 28px!important;
  display: flex;
  align-items: center;
  cursor: pointer;
}
</style>