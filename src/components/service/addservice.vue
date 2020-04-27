<template>
  <div>
    <div>
      <el-radio-group v-model="type" class="choose" @change="typeChange">
        <el-radio-button label="uds">UDS</el-radio-button>
        <el-radio-button label="group">GROUP</el-radio-button>
      </el-radio-group>
    </div>
    <div v-if="type=='uds'">
      <div class="subheader">Serivce</div>
      <el-select
        v-model="itemIndex"
        placeholder="请选择"
        class="choose"
        @change="itemChange"
        style="width:100%"
      >
        <el-option v-for="(item,key) in config" :key="key" :label="item.name" :value="key" >
          <span style="float: left">{{ item.name }}</span>
          <span style="float: right; color: #8492a6; font-size: 13px">0x{{ item.value.toString(16) }}</span>
        </el-option>
      </el-select>

      <Service :config="config[itemIndex]" @additem="addItem" v-if="refresh" />
    </div>
    <div v-else>
      <div class="subheader">Group</div>
      
      <el-select
        v-model="itemIndex"
        placeholder="请选择"
        class="choose"
        @change="itemChange"
        style="width:90%"
      >
        <el-option v-for="(item,key) in group" :key="key" :label="key" :value="key">
        </el-option>
      </el-select>
      <el-button icon="el-icon-refresh-right" type="success" plain style="margin-left:20px" size="small" @click="typeChange('group')"></el-button>
      <Group :config="group[itemIndex]" @additem="addItem" v-if="refresh" />
    </div>
  </div>
</template>

<script>
import config from "./service.js";
import Service from "./service.vue";
import Group from "./group.vue";
const { ipcRenderer } = require("electron");
export default {
  components: {
    Service,
    Group
  },
  data() {
    return {
      config: config['uds'],
      group:{},

      type: "uds",
      itemIndex: 0,
      refresh: true,
    };
  },
  props: ["mode"],
  methods: {
    addItem(val) {
      //console.log(val)
      var item = JSON.parse(JSON.stringify(val));
      if (this.mode === "doip") {
        this.$store.commit("doipTableAdd", item);
      } else if (this.mode === "can") {
        this.$store.commit("canTableAdd", item);
      }
      this.$emit("additem");
    },
    typeChange(val) {
      
      if(val==='group'){
        this.itemIndex=''
        var data=ipcRenderer.sendSync('readGroup');
        var map=new Map(JSON.parse(data));
        this.group={}
        for (let [key, value] of map) {
          this.group[key]=JSON.parse(value)
        }
      }else{
        this.itemIndex=0
      }
    },
    itemChange() {
      this.refresh = false;
      this.$nextTick(() => {
        this.refresh = true;
      });
    }
  }
};
</script>

<style>
.choose {
  margin-bottom: 20px;
}
.subheader {
  font-family: Impact, Haettenschweiler, "Arial Narrow Bold", sans-serif;
  font-size: 25px;
  margin-bottom: 10px;
}
</style>