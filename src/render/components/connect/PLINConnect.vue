<template>
  <div class="connect">
   
    <el-row style="margin:0px">
      <el-col :span="11">
        <div class="title">Device:</div>
      </el-col>
    </el-row>
    <el-row>
      <el-col :span="24">
        <el-select v-model="device" placeholder="Deivce" :disabled="connected" style="width:100%">
          <el-option
            v-for="item in deviceList"
            :key="item.value"
            :label="item.label"
            :value="item.value"
          ></el-option>
        </el-select>
      </el-col>
    </el-row>
    <div>
      <el-row style="margin:0px">
        <el-col :span="11">
          <div class="title">Speed:</div>
        </el-col>
      </el-row>
      <el-row>
        <el-col :span="24">
          <el-select v-model="speed" placeholder="Speed" :disabled="connected" style="width:100%">
            <el-option
              v-for="item in linSpeed"
              :key="item.value"
              :label="item.label"
              :value="item.value"
            ></el-option>
          </el-select>
        </el-col>
      </el-row>
    </div>
    <el-row style="text-align:right">
      <el-col :span="4" :offset="20" v-if="!connected">
        <el-button type="primary" plain @click="connect">Connect</el-button>
      </el-col>
      <el-col :span="4" :offset="20" v-else>
        <el-button type="danger" plain @click="disconnect">Disconnect</el-button>
      </el-col>
    </el-row>
    <el-divider></el-divider>
    <div>
      <el-form :model="tpConfig" :disabled="!connected">
        <el-form-item label="Name:" required>
          <el-input v-model="tpConfig.name" placeholder="Name" maxlength="20" style="width:180px"></el-input>
        </el-form-item>
        <el-form-item label="Send NAD:" required>
          <el-input v-model="tpConfig.sendNad" placeholder="Send NAD" maxlength="20" style="width:180px"></el-input>
        </el-form-item>
        <el-form-item label="Receive NAD:" required>
          <el-input v-model="tpConfig.recvNad" placeholder="Receive NAD" maxlength="20" style="width:180px"></el-input>
        </el-form-item>
        <el-form-item label="ST Min:" required>
         <el-input v-model="tpConfig.stMin" placeholder="ST Min" maxlength="20" style="width:180px"></el-input>
        </el-form-item>
        <el-form-item label="P2(ms):" required>
         <el-input v-model="tpConfig.p2" placeholder="P2" maxlength="20" style="width:180px"></el-input>
        </el-form-item>
        <el-form-item label="P2*(ms):" required>
         <el-input v-model="tpConfig.p2Opt" placeholder="P2" maxlength="20" style="width:180px"></el-input>
        </el-form-item>
        <el-form-item style="text-align:right">
          <el-button @click="setupAddr" type="primary" plain>ADD</el-button>
        </el-form-item>
      </el-form>
    </div>
    <el-table size="mini" :data="addrTable" style="width: 100%" empty-text="No Data">
      <el-table-column type="index" width="50" align="center"></el-table-column>
      <el-table-column prop="name" label="Name" width="100" align="center"></el-table-column>
      <el-table-column prop="sendNad" label="Send NAD" width="250" align="center">
        <template slot-scope="scope">
          <el-tag size="medium" type="info">{{scope.row.sendNad}}</el-tag>
        </template>
      </el-table-column>
      <el-table-column prop="recvNad" label="Receive NAD" width="250" align="center">
        <template slot-scope="scope">
          <el-tag size="medium" type="info">{{scope.row.recvNad}}</el-tag>
        </template>
      </el-table-column>
      <el-table-column prop="stMin" label="ST Min" width="100" align="center"></el-table-column>
      <el-table-column prop="p2" label="P2" width="100" align="center"></el-table-column>
      <el-table-column prop="p2Opt" label="P2*" width="100" align="center"></el-table-column>
      <el-table-column label="Action" align="center" fixed="right">
        <template slot-scope="scope">
          <el-button
            size="mini"
            type="danger"
            :disabled="!connected"
            @click="handleDelete(scope.$index)"
          >Delete</el-button>
        </template>
      </el-table-column>
    </el-table>
    <el-tag type="info" style="width:100%;margin-top:10px">{{linDllVersion}}</el-tag>
  </div>
</template>
<script>
//import PCANTP from './../../build/Release/PCANTP.node'

const { ipcRenderer } = require("electron");
export default {
  data: function() {
    return {
      tpConfig: {
        name: "defaultName1",
        sendNad: "0",
        recvNad: "0",
        stMin: "0",
        p2: "50",
        p2Opt: "50",
      },
      linDllVersion:'',
      device: 1,
      deviceList: [
        {
          label: "PEAK-USB1",
          value: 1
        },
        {
          label: "PEAK-USB2",
          value: 2
        }
      ],
      speed: 19200,
      linSpeed: [
        {
          label: "9600",
          value: 9600
        },
        {
          label: "19200",
          value: 19200
        },
      ],

    };
  },
  mounted(){
    this.linDllVersion= ipcRenderer.sendSync("linDllVersion");
  },
  computed: {
    addrTable: function() {
      return this.$store.state.linAddrTable;
    },
    connected: function() {
      return this.$store.state.linConnect;
    },
  },
  methods: {
    handleDelete(index) {
      this.$store.commit("linAddrDelete", index);
    },
    setupAddr() {
      var item = JSON.parse(JSON.stringify(this.tpConfig));
      for (var i in this.addrTable) {
        if (this.addrTable[i].name === item.name) {
          this.$notify.error({
            title: "Error",
            message: "Name Exist"
          });
          return;
        }
      }
      if (item.sendNad === item.recvNad) {
        this.$notify.error({
          title: "Error",
          message: "Send NAD Equal Receive NAD"
        });
        return;
      }
      item.sendNad = parseInt(item.sendNad, 16);
      item.recvNad = parseInt(item.recvNad, 16);

      this.$store.commit("linAddrAdd", item);
    },
    connect() {
      let err;
      err = ipcRenderer.sendSync("linConnect", [
        this.device,
        this.speed
      ]);
     
      if (err.err === 0) {
        // this.connected = true
        this.$store.commit("linChange", true);
      } else {
        this.$notify.error({
          title: "Error",
          message: err.msg
        });
      }
    },
    disconnect() {
      let err = ipcRenderer.sendSync("linDisconnect", this.device);
      if (err.err === 0) {
        this.$store.commit("linChange", false);
        this.$store.commit("linAddrLoad", []);
      } else {
        this.$notify.error({
          title: "Error",
          message: err.msg
        });
      }
    }
  }
};
</script>
<style>
.el-row {
  margin-bottom: 10px;
}
.header {
  font-weight: 800;
}
.title {
  line-height: 18px;
  font-weight: 550;
}
</style>
