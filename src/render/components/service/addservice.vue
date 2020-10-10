<template>
  <div>
    <div v-if="!editService">
      <el-radio-group v-model="type" class="choose" @change="typeChange">
        <el-radio-button label="uds">UDS</el-radio-button>
        <el-radio-button label="group">GROUP</el-radio-button>
      </el-radio-group>
      <div v-if="type=='uds'">
        <div class="subheader">Serivce</div>
        <el-select
          v-model="itemIndex"
          placeholder="Service"
          class="choose"
          @change="itemChange"
          style="width:100%"
        >
          <el-option v-for="(item,key) in config" :key="key" :label="item.name" :value="key">
            <span style="float: left">{{ item.name }}</span>
            <span
              style="float: right; color: #8492a6; font-size: 13px"
            >0x{{ item.value.toString(16) }}</span>
          </el-option>
        </el-select>

        <Service :config="config[itemIndex]" @additem="addItem" v-if="refresh" />
      </div>
      <div v-else>
        <div class="subheader">Group</div>

        <el-select
          v-model="itemIndex"
          placeholder="Group Service"
          class="choose"
          @change="itemChange"
          style="width:80%"
        >
          <el-option v-for="(item,key) in group" :key="key" :label="key" :value="key"></el-option>
        </el-select>
        <el-button
          icon="el-icon-refresh-right"
          type="success"
          plain
          style="margin-left:20px"
          size="small"
          @click="typeChange('group')"
        ></el-button>
        <Group :config="group[itemIndex]" @additem="addItem" v-if="refresh" />
      </div>
    </div>
    <div v-else>
      <div v-if="service.type=='uds'">
        <Service
          :config="service.cfg"
          :input="service.val"
          change
          @edititem="editItem"
          ref="editService"
          v-if="refresh"
        />
      </div>
      <div v-else>
        <Group
          :config="service.cfg"
          :input="service.val"
          @edititem="editItem"
          ref="editGroup"
          change
        />
      </div>
    </div>
  </div>
</template>

<script>
import config from "./service.js";
import Service from "./service.vue";
import Group from "./group.vue";
import pregroup from "./predefgroup.js";
const { ipcRenderer } = require("electron");
export default {
  components: {
    Service,
    Group
  },
  data() {
    return {
      config: config["uds"],
      group: {},
      type: "uds",
      itemIndex: 0,
      refresh: true
    };
  },

  props: {
    mode: {
      type: String,
      default: function() {
        return "doip";
      }
    },
    editService: {
      type: Boolean,
      default: function() {
        return false;
      }
    },
    service: {
      type: Object,
      default: function() {
        return {
          type: "uds",
          cfg: "",
          val: ""
        };
      }
    }
  },
  methods: {
    addItem(val) {
      this.$emit("additem",val);
    },
    editItem(val) {
      this.$emit("edititem", val);
    },
    isDirty() {
      var dirty = false;
      if (this.editService) {
        if (this.service.type == "uds") {
          let tmp = this.$refs.editService.generateData();
          if (tmp["func"] != this.service.val["func"]) {
            dirty = true;
          } else {
            if (
              JSON.stringify(tmp["payload"]) !=
              JSON.stringify(this.service.val["payload"])
            ) {
              dirty = true;
            }
          }
        } else {
          let tmp = this.$refs.editGroup.generateData();
          let subtable = this.service.val.subtable;
          for (var i in subtable) {
            if (tmp[i].func != subtable[i].func) {
              dirty = true;
            } else {
              if (
                JSON.stringify(tmp[i].payload) !=
                JSON.stringify(subtable[i].payload)
              ) {
                dirty = true;
              }
            }
            if(dirty){
              break
            }
          }
        }
      }
      return dirty;
    },
    typeChange(val) {
      if (val === "group") {
        this.itemIndex = "";
        var data = ipcRenderer.sendSync("readGroup");
        var map = new Map(JSON.parse(data));
        this.group = {};
        for (let [key, value] of map) {
          this.group[key] = JSON.parse(value);
        }
        for (var i in pregroup) {
          this.group[pregroup[i][0]] = JSON.parse(pregroup[i][1]);
        }
      } else {
        this.itemIndex = 0;
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