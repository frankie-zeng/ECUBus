<template>
  <div>
    <div>
      <el-radio-group v-model="type" class="choose" @change="itemChange">
        <el-radio-button label="uds">UDS</el-radio-button>
        <el-radio-button label="doip" v-if="mode==='doip'">DOIP</el-radio-button>
      </el-radio-group>
    </div>
    <div class="choose">
      <el-select
        v-model="itemIndex"
        placeholder="请选择"
        class="choose"
        @change="itemChange"
        style="width:100%"
      >
        <el-option v-for="(item,key) in config[type]" :key="key" :label="item.name" :value="key">
          <span style="float: left">{{ item.name }}</span>
          <span
            style="float: right; color: #8492a6; font-size: 13px"
          >0x{{ item.value.toString(16) }}</span>
        </el-option>
      </el-select>
    </div>

    <Service :config="config[type][itemIndex]" @additem="addItem" v-if="refresh" />
    
  </div>
</template>

<script>

import config from "./service.js";
import Service from "./service.vue";
export default {
  components: {
    Service
  },
  data() {
    return {
      config: config,
      type: "uds",
      itemIndex: 0,
      refresh: true,
    };
  },
  props: ["mode"],
  methods: {
    addItem(val) {
      //console.log(val)
      if (this.mode === "doip") {
        this.$store.commit("doipTableAdd", val);
      } else if (this.mode === "can") {
        this.$store.commit("canTableAdd", val);
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
</style>