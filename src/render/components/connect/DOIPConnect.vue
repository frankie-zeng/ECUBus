<template>
  <div class="connect">
    <div style="margin-top:20px">
      <el-form :model="req" ref="devForm" :rules="devRule" label-width="100px" size="mini">
        <el-form-item label="Request type:" prop="type">
          <el-radio-group v-model="req.type">
            <el-radio-button label="NULL"></el-radio-button>
            <el-radio-button label="VIN"></el-radio-button>
            <el-radio-button label="EID"></el-radio-button>
          </el-radio-group>
        </el-form-item>
        <el-form-item label="VIN:" v-if="req.type==='VIN'" prop="vin" required>
          <el-input placeholder="Input VIN,ASCII" v-model="req.vin" maxlength="17" show-word-limit />
        </el-form-item>
        <el-form-item label="EID:" v-if="req.type==='EID'" prop="eid" required>
          <el-input placeholder="Input EID,HEX" v-model="req.eid" maxlength="12" show-word-limit />
        </el-form-item>
        <el-form-item label="Multicast:" prop="multicast">
          <el-input v-model="req.multicast" placeholder="255.255.255.255"></el-input>
        </el-form-item>
        <el-form-item style="text-align:right">
          <el-button @click="findDevice" type="primary">Search Device</el-button>
        </el-form-item>
      </el-form>
      <el-divider></el-divider>
    </div>
    <el-table size="mini" :data="deviceList" style="width: 100%" empty-text="No Device">
      <el-table-column prop="ip" label="IP Address" width="150" align="center"></el-table-column>
      <el-table-column prop="vin" label="VIN" width="150" align="center"></el-table-column>
      <el-table-column prop="logicalAddr" label="Logical Address" width="150" align="center"></el-table-column>
      <el-table-column prop="eid" label="EID" width="120" align="center">
        <template slot-scope="scope">0x{{scope.row.eid}}</template>
      </el-table-column>
      <el-table-column prop="gid" label="GID" width="120" align="center">
        <template slot-scope="scope">0x{{scope.row.gid}}</template>
      </el-table-column>
      <el-table-column prop="fAction" label="Action" width="100" align="center"></el-table-column>
      <el-table-column prop="syncStatus" label="Sync" width="100" align="center"></el-table-column>
      <el-table-column label="Action" align="center" fixed="right">
        <template slot-scope="scope">
          <el-button size="mini" type="danger" @click="connectDevice(scope.row)">Connect</el-button>
        </template>
      </el-table-column>
    </el-table>
    <div style="margin-top:30px">
      <el-form :model="active" ref="activeForm" :rules="activeRule" label-width="100px" size="mini">
        <el-form-item label="Timeout" prop="timeout">
          <el-input v-model.number="active.timeout" />
        </el-form-item>
        <el-form-item label="SA:" prop="sa">
          <el-input placeholder="Source Address" v-model="active.sa" />
        </el-form-item>
        <el-form-item label="Active Type:" prop="activeType">
          <el-select v-model="active.activeType" style="width:100%" allow-create filterable>
            <el-option label="Default" value="00"></el-option>
            <el-option label="WWH-OBD" value="01"></el-option>
            <el-option label="Central security" value="e0"></el-option>
          </el-select>
        </el-form-item>
        <el-form-item label="OEM Special:" prop="option">
          <el-input placeholder v-model="active.option" maxlength="8" show-word-limit />
        </el-form-item>
      </el-form>
    </div>
    <el-table size="mini" :data="addrTable" style="width: 100%">
      <el-table-column prop="name" label="VIN" align="center" width="150"></el-table-column>
      <el-table-column prop="SA" label="Source" align="center"></el-table-column>
      <el-table-column prop="TA" label="Target" align="center"></el-table-column>
      <el-table-column prop="code" label="Code" align="center">
        <template slot-scope="scope">0x{{scope.row.code.toString(16)}}</template>
      </el-table-column>
      <el-table-column label="Action" align="center" fixed="right">
        <template slot-scope="scope">
          <el-button
            size="mini"
            type="danger"
            @click="disconnectDevice(scope.row,scope.$index)"
          >Close</el-button>
        </template>
      </el-table-column>
    </el-table>
  </div>
</template>
<script>
//import PCANTP from './../../build/Release/PCANTP.node'

const { ipcRenderer } = require("electron");
export default {
  mounted() {
    ipcRenderer.on("doipDeviceFound", (event, val) => {
      var item = val[0];
      item.ip = val[1].address;
      this.deviceList.push(item);
    });
    ipcRenderer.on("doipTcpStatus", (event, val) => {
      if (val.err != 0) {
        this.$notify.error({
          title: "Error",
          message: val.msg
        });
        if (val.err < -1) {
          this.$store.commit("doipAddrDeleteKey", val.key);
        }
      } else {
        var testerAddr = val.data.testerAddr;
        var entityAddr = val.data.entityAddr;
        var i;
        var found = false;
        for (i in this.deviceList) {
          if (this.deviceList[i].logicalAddr === entityAddr) {
            found = true;
            break;
          }
        }
        if (found) {
          this.$store.commit(
            "doipAddrAdd",
            JSON.parse(
              JSON.stringify({
                name: this.deviceList[i].vin,
                SA: testerAddr,
                TA: entityAddr,
                key: val.key,
                code: val.data.code
              })
            )
          );
          this.$store.commit("doipChange", true);
        } else {
          this.$notify.error({
            title: "Error",
            message: "Address Unmatch"
          });
          ipcRenderer.send("doipTcpDisconnectWithKey", val.key);
        }
      }
    });
  },
  destroyed() {
    ipcRenderer.removeAllListeners("deviceFound");
    ipcRenderer.removeAllListeners("doipTcpStatus");
  },
  data: function() {
    return {
      active: {
        activeType: "00",
        timeout: 2000,
        option: ""
      },
      deviceList: [],
      req: {
        type: "NULL"
      },
      activeRule: {
        timeout: [{ required: true, type: "number", trigger: "blur" }],
        sa: [
          { required: true, message: "Please Input SA", trigger: "blur" }
        ],
        activeType: [
          {
            required: true,
            min: 1,
            max: 2,
            message: "Please Input ACTIVE-TYPE",
            trigger: "blur"
          }
        ],
        option: [
          {
            pattern: /^([0-9a-fA-F]{2})+$/,
            message: "Please Input HEX format",
            trigger: "change"
          }
        ]
      },
      devRule: {
        vin: [{ min: 17, max: 17, message: "Length Need 17", trigger: "blur" }],
        eid: [
          {
            pattern: /^([0-9a-fA-F]{2})+$/,
            message: "Please Input HEX format",
            trigger: "change"
          },
          { min: 12, max: 12, message: "Length Need 6 Bytes", trigger: "blur" }
        ],
        multicast: [
          {
            required: true,
            pattern: /^((25[0-5]|2[0-4]\d|1\d{2}|[1-9]?\d)\.){3}(25[0-5]|2[0-4]\d|1\d{2}|[1-9]?\d)$/,
            message: "Please Input IP Address",
            trigger: "change"
          }
        ]
      }
    };
  },
  computed: {
    addrTable: function() {
      return this.$store.state.doipAddrTable;
    }
  },
  methods: {
    findDevice() {
      this.$refs.devForm.validate(valid => {
        if (valid) {
          this.deviceList = [];
          ipcRenderer.send("doipDeviceFind", this.req);
        }
      });
    },
    connectDevice(val) {
      this.$refs.activeForm.validate(valid => {
        if (valid) {
          ipcRenderer.send("doipTcpConnect", [val, this.active]);
        }
      });
    },
    disconnectDevice(val, index) {
      ipcRenderer.send("doipTcpDisconnectWithKey", val.key);
      this.$store.commit("doipAddrDelete", index);
      if (this.addrTable.length == 0) {
        this.$store.commit("doipChange", false);
      } else {
        this.$store.commit("doipChange", true);
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
