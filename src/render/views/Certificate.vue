<template>
  <div>
    <el-page-header @back="goBack" content="Certificate Tool" class="header" title></el-page-header>
    <el-tree :data="data" :props="defaultProps"  @node-click="handleNodeClick"></el-tree>
    <div class="dumpDis">
      <div style="position: relative; padding-bottom: 1em;">
        <div id="dump" style="position: absolute; right: 0px;"></div>
        <div id="tree"></div>
      </div>
    </div>
  </div>
</template>
<script>
import ASN1 from "@lapo/asn1js";
import Hex from "@lapo/asn1js/hex";
import oids from "@lapo/asn1js/oids";

const testStr =
  "308205653082034da00302010202144f3ce7423506cdb419a8d334cd8e594f7842cadf300d06092a864886f70d01010b05003042310b300906035504061302434e3110300e06035504080c076672616e6b69653112301006035504070c0963686f6e6771696e67310d300b060355040a0c0467706973301e170d3230303931343031323933335a170d3231303931343031323933335a3042310b300906035504061302434e3110300e06035504080c076672616e6b69653112301006035504070c0963686f6e6771696e67310d300b060355040a0c046770697330820222300d06092a864886f70d01010105000382020f003082020a0282020100edb122db4054a0dc6310ac3a14cdb5bd541101cc8dd30d260965a4f3211df2ad80f85225d20345ecc94a8ceb1ce90c84a11ac68658edb5df8fc0f6d7ad0397a295482aebe48710f069b1cdd986abf1ebdeaaa214f227363b0f9fa1b54bd91aa20ef62dc056ba63b41946929054cfd27eaac5fbbed31b02b02bd249b75f16096335ba0bd68e5813605794fd35a4c048568dbc7efbe81df78eea35e93a924094b2a977ef51c2fcdfe73ff8aafb4c5b03bd1668b4de22469f9a724b5a2f7c8f961ce71ff25341ea925b93cfd62143b2403f325f1d92a9d327368b67c64718a8738939e8ff1f4b338990e5f2a760da043212892ddae46875c8448d96333aca55a5172f279b02248d6aee87e83bc30a667e553146e778c9bacc3478ef69b59ba651a6fb74f681b444ff60d073a0fc57feeb7b1aa381665e63259f6d1392bbb0f1b39f0221bd561f47e1951ccef615b761dc16bc66d0eacd544c877e7b34065a40faff948f7d7fcb7ad17aebe27903eb633303cfbfdb562b78c81c57f0b2cb552aec6763d86616441a61cbfee1db7b4c2e82d07a03551dea9982d03648965bd7745f10f0cb908600d93cdea13d22f22aff2e96e327d7fdc4a70d0deee9d4203969304fcd422f055d8d8d987bd4f9addeb3f578381c25be82eb0be99e8e725db36e249e014daaaea695f5e90281f62cb828f0cc0b736c59cfdfabbabffe27506a5ace910203010001a3533051301d0603551d0e04160414d4da2d180115d34fa08246b0ee3af0e87319414e301f0603551d23041830168014d4da2d180115d34fa08246b0ee3af0e87319414e300f0603551d130101ff040530030101ff300d06092a864886f70d01010b0500038202010093affa6404fdc721139fd283631452677f19bd9e5ed6737aae173e8e37713cc82b4602054273f74757e84fd8a92ffaa365012e513e19654ff4436cb290b4e528c6f8500e4d9520a27f94a9ceb1d807df5c61e2880ae497b4b3d3aeab7c45c8df3399f297fc63d40c1cfd027331f248e8f4ef4355adde633354cac80dae16d62f7f866cd039c87d111e1f36807994acdb6e4deaaabfa8aa8124e9b4a2227540e4da89c46d9fe7dee270385c8bd2f8d5654fb2c271620ef8994d61c75a825caa91a2a360612f39155069d3f7e17be9aaf5d804bc1c04a715f52b567a08754342e4553965d1993e18f8ac4072f53832bbdf6b25f4c7efae367a88f4918593f1166519274477884ba9b388b79fb2e3ea6245d3520912294fbd83cb8f70ebc6a087d73eb6b4ec554f76e4a2891928e9005818d6657930d88ad3bb7d9a5c946eeb3a50a9ad6935968119d87673684664bf04001654a6f95ee9ecffe061603705a77f9e5278c12008e7c5b5364e6a2807f0c64b2e84e2da2b8620c50614103ef4a918592f34a3ab685dabd43ba7b3b4d880834e382a5b0657572f35cc1012805fea40751051dfb01928223966d5858bbedd8fc8c8897883462596d3126421558821582bb039233db6a979c32d7cc8f9a2b788cd313dce3ac5b4dbd2f843ff8ecbd145c2d20de3872220e38d723f1527e9d431f8ea95d169d97196fc9fabaed62ab16356";
var lineLength = 80,
  contentLength = 8 * lineLength,
  DOM = {
    ellipsis: "\u2026",
    tag: function (tagName, className) {
      var t = document.createElement(tagName);
      t.className = className;
      return t;
    },
    text: function (str) {
      return document.createTextNode(str);
    },
    space: function () {
      var t = document.createElement("span");
      t.className = "spaces";
      t.innerHTML = " ";
      return t;
    },
    breakLines: function (str, length) {
      var lines = str.split(/\r?\n/),
        o = "";
      for (var i = 0; i < lines.length; ++i) {
        var line = lines[i];
        if (i > 0) o += "\n";
        while (line.length > length) {
          o += line.substring(0, length);
          o += "\n";
          line = line.substring(length);
        }
        o += line;
      }
      return o;
    },
  };

export default {
  data() {
    return {
      asn1: ASN1.decode(Hex.decode(testStr)),
      tree: "",
      data:[],
      defaultProps: {
          children: 'children',
          label: 'label'
        }
    };
  },
  mounted() {
    console.log(this.asn1);
    this.tree = document.getElementById("tree");
    this.tree.appendChild(this.toDom(this.asn1));
    this.data=this.toTree(this.asn1)
    // console.log(this.toTree(this.asn1))
  },
  methods: {
    goBack() {
      this.$router.push("/");
    },
    handleNodeClick(e){
        console.log(e)
    },
    toTree(obj) {
      var isOID =
        typeof oids === "object" &&
        obj.tag.isUniversal() &&
        obj.tag.tagNumber == 0x06;
      var node = {}
      node.asn1 = obj;
      node.label= obj.typeName().replace(/_/g, " ");
    
      var content = obj.content();
      if (content !== null) {
       
        var shortContent;
        if (isOID) content = content.split("\n", 1)[0];
        shortContent =
          content.length > lineLength
            ? content.substring(0, lineLength) + DOM.ellipsis
            : content;
        node.label+=' '+shortContent
        if (isOID) {
          var oid = oids[content];
          if (oid) {
            if (oid.d) {
                node.info+=oid.d
            }
            if (oid.c) {
              node.info+=oid.d
            }
          }
        }
      }
      node.offset=obj.stream.pos
      node.header=obj.header
      node.length=obj.length
    
      
      var children=[]
      if (obj.sub !== null) {
      
        for (var i = 0, max = obj.sub.length; i < max; ++i)
          children.push(this.toTree(obj.sub[i])[0]);
      }
      node.children=children
      return [node];
    },
    toDom(obj, spaces) {
      spaces = spaces || "";
      var isOID =
        typeof oids === "object" &&
        obj.tag.isUniversal() &&
        obj.tag.tagNumber == 0x06;
      var node = DOM.tag("div", "node");
      node.asn1 = obj;
      var head = DOM.tag("div", "head");
      head.innerHTML =
        "<span class='spaces'>" +
        spaces +
        "</span>" +
        obj.typeName().replace(/_/g, " ");
      var content = obj.content(contentLength);
      if (content !== null) {
        var preview = DOM.tag("span", "preview"),
          shortContent;
        if (isOID) content = content.split("\n", 1)[0];
        shortContent =
          content.length > lineLength
            ? content.substring(0, lineLength) + DOM.ellipsis
            : content;
        preview.appendChild(DOM.space());
        preview.appendChild(DOM.text(shortContent));
        if (isOID) {
          var oid = oids[content];
          if (oid) {
            if (oid.d) {
              preview.appendChild(DOM.space());
              var oidd = DOM.tag("span", "oid description");
              oidd.appendChild(DOM.text(oid.d));
              preview.appendChild(oidd);
            }
            if (oid.c) {
              preview.appendChild(DOM.space());
              var oidc = DOM.tag("span", "oid comment");
              oidc.appendChild(DOM.text("(" + oid.c + ")"));
              preview.appendChild(oidc);
            }
          }
        }
        head.appendChild(preview);
        content = DOM.breakLines(content, lineLength);
        content = content.replace(/</g, "&lt;");
        content = content.replace(/\n/g, "<br>");
      }
      node.appendChild(head);
      obj.node = node;
      obj.head = head;
      var value = DOM.tag("div", "value");
      var s = "Offset: " + obj.stream.pos + "<br>";
      s += "Length: " + obj.header + "+";
      if (obj.length >= 0) s += obj.length;
      else s += -obj.length + " (undefined)";
      if (obj.tag.tagConstructed) s += "<br>(constructed)";
      else if (
        obj.tag.isUniversal() &&
        (obj.tag.tagNumber == 0x03 || obj.tag.tagNumber == 0x04) &&
        obj.sub !== null
      )
        s += "<br>(encapsulates)";
      //TODO if (this.tag.isUniversal() && this.tag.tagNumber == 0x03) s += "Unused bits: "
      if (content !== null) {
        s += "<br>Value:<br><b>" + content + "</b>";
        if (isOID && oid) {
          if (oid.d) s += "<br>" + oid.d;
          if (oid.c) s += "<br>" + oid.c;
          if (oid.w) s += "<br>(warning!)";
        }
      }
      value.innerHTML = s;
      node.addEventListener("click", this.valueClick);
      node.appendChild(value);
      var sub = DOM.tag("div", "sub");
      if (obj.sub !== null) {
        spaces += "\xA0 ";
        for (var i = 0, max = obj.sub.length; i < max; ++i)
          sub.appendChild(this.toDom(obj.sub[i],spaces));
      }
      node.appendChild(sub);
      head.onclick = function () {
        node.className =
          node.className == "node collapsed" ? "node" : "node collapsed";
      };
      return node;
    },
  },
};
</script>
<style>
@import "./Certificate.css";
.dumpDis {
  margin: 30px;
}
</style>