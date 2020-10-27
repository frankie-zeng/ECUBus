<template>
  <div>
    <el-dialog
      title="Add UDS Serivce"
      :visible.sync="showService"
      width="80%"
      :close-on-press-escape="false"
    >
      <div class="connect">
        <AddService @additem="serviceAddedCb" :mode="mode" />
      </div>
    </el-dialog>
    <el-dialog
      title="Combine Multi Service Into Group"
      :visible.sync="showGroup"
      width="80%"
    >
      <div class="connect">
        <Group :mode="mode" @added="groupAddedCb" :table="schs[schIndex].services"/>
      </div>
    </el-dialog>

    <el-dialog
      title="Edit Service"
      :visible.sync="cd1"
      width="80%"
      :close-on-press-escape="false"
      :before-close="editClose"
    >
      <AddService
        editService
        :mode="mode"
        :service="service"
        v-if="refresh"
        @edititem="editItem"
        ref="addService"
      />
    </el-dialog>

    <el-dialog title="User function" :visible.sync="cd" width="80%">
      <div style="text-align: center">
        <el-pagination
          layout="prev, pager, next"
          hide-on-single-page
          :total="codeNum * 10"
          @current-change="showNewCode"
        ></el-pagination>
      </div>
      <div>function(writeData,readData){</div>
      <codemirror
        v-model="jsFn[codeIndex]"
        :options="codeOption"
        ref="cmEditor"
      />
      <div>}</div>
    </el-dialog>
    <div v-for="(sch, index) in schs" :key="index">
      <el-row style="text-align: left; margin-top: 10px">
        <el-col :span="6">Schedule Name: </el-col>
        <el-col :span="6" :offset="1"> Address: </el-col>
      </el-row>
      <el-row style="text-align: right; margin-right: 20px">
        <el-col :span="6"
          ><el-input
            v-model="sch.name"
            placeholder="name"
            size="mini"
          ></el-input
        ></el-col>
        <el-col :span="6" :offset="1">
          <el-select
            v-model="sch.addr"
            placeholder="Address"
            style="width: 100%"
            size="mini"
          >
            <el-option
              v-for="(item, key) in addrTable"
              :key="key"
              :label="item.name"
              :value="key"
            >
              <span style="float: left">{{ item.name }}</span>
              <span style="float: right; color: #8492a6; font-size: 13px"
                >SA:{{ item.SA }},TA:{{ item.TA }}</span
              >
            </el-option>
          </el-select>
        </el-col>
        <el-button
          v-if="index != 0"
          icon="el-icon-delete"
          @click="deleteSch(index)"
          size="mini"
          type="danger"
          >Delete Schedule</el-button
        >
        <el-button
          icon="el-icon-wallet"
          @click="addGroup(index)"
          size="mini"
          type="primary"
          >Save Group</el-button
        >
        <el-button
          icon="el-icon-plus"
          @click="addService(index)"
          size="mini"
          type="primary"
          >Add Service</el-button
        >
      </el-row>

      <el-table
        size="small"
        ref="srvTable"
        row-key="date"
        border
        :data="sch.services"
        max-height="600px"
        style="width: 100%; text-align: center"
        empty-text="No Service"
        :row-class-name="tableRowClassName"
        @selection-change="handleSelectionChange"
      >
        <el-table-column
          type="selection"
          width="55"
          align="center"
        ></el-table-column>
        <el-table-column prop="service" label="Service Info" width="300">
          <template slot-scope="scope">
            <div v-if="scope.row.type == 'uds'">
              {{ scope.row.service.name }} (0X{{
                scope.row.service.value.toString(16)
              }})
            </div>
            <div v-else>
              <el-tag size="mini">Group:</el-tag>
              <strong>{{ scope.row.service.name }}</strong>
            </div>
          </template>
        </el-table-column>
        <el-table-column label="Suppress" width="76" align="center">
          <template slot-scope="scope" v-if="scope.row.type == 'uds'">
            <i
              class="el-icon-circle-check"
              v-if="scope.row.payload[0].suppress"
              style="color: green"
            ></i>
            <i class="el-icon-circle-close" v-else style="color: red"></i>
          </template>
        </el-table-column>
        <el-table-column prop="payload" label="Payload">
          <template slot-scope="scope">
            <div v-if="scope.row.payload && scope.row.type == 'uds'">
              <div v-for="(item, key) in scope.row.payload" :key="key">
                <el-tag size="mini">{{ item.name }}</el-tag>
                : {{ item[item.name] }}
              </div>
            </div>
            <div
              v-else-if="scope.row.subtable && scope.row.type == 'group'"
              style="height: 100px; overflow: auto"
            >
              <div v-for="(item, key) in scope.row.subtable" :key="key">
                <div v-if="item.payload">
                  <div v-for="(subitem, subkey) in item.payload" :key="subkey">
                    <el-tag size="mini">{{ key + "-" + subitem.name }}</el-tag>
                    : {{ subitem[subitem.name] }}
                    <span v-if="subitem.type == 'subfunction'">
                      <i
                        class="el-icon-circle-check"
                        v-if="subitem.suppress"
                        style="color: green"
                      ></i>
                      <i
                        class="el-icon-circle-close"
                        v-else
                        style="color: red"
                      ></i>
                    </span>
                  </div>
                </div>
              </div>
            </div>
            <div v-else>NULL</div>
          </template>
        </el-table-column>

        <el-table-column prop="func" label="Script" width="100" align="center">
          <template slot-scope="scope">
            <el-button
              type="text"
              icon="el-icon-document"
              @click="showCode(scope.row)"
            ></el-button>
          </template>
        </el-table-column>
        <el-table-column
          fixed="right"
          label="Action"
          width="150"
          align="center"
        >
          <template slot="header">
            <el-button
              size="mini"
              :disabled="selectTable.length == 0"
              @click="deleteMulti(index)"
              icon="el-icon-delete"
              type="text"
              >Selected</el-button
            >
          </template>
          <template slot-scope="scope">
            <!-- <h1 v-if="scope.$index==tableErrorIndex">error</h1> -->
            <div
              v-if="
                scope.$index == tableErrorIndex[1] - 1 &&
                index == tableErrorIndex[0]
              "
              class="table_error"
            >
              <i class="el-icon-error" @click="closeError"></i>
            </div>
            <el-button
              type="danger"
              size="mini"
              icon="el-icon-delete"
              circle
              @click="deleteService(index, scope.$index)"
              :disabled="running"
            ></el-button>
            <el-button
              type="warning"
              size="mini"
              icon="el-icon-edit-outline"
              circle
              @click="editService(index, scope.$index, scope.row)"
              :disabled="running"
            ></el-button>
          </template>
        </el-table-column>
      </el-table>
      <el-divider></el-divider>
    </div>
    <el-button
      style="width: 100%; margin-top: 20px"
      @click="addSch"
      type="primary"
      plain
      ><i class="el-icon-plus"></i
    ></el-button>
  </div>
</template>
<script>
import Sortable from "sortablejs";
import config from "./service.js";
import AddService from "./addservice.vue";
import Group from "./groupservice.vue";
const { ipcRenderer } = require("electron");
import pregroup from "./predefgroup.js";
export default {
  components: {
    AddService,
    Group,
  },
  data: function () {
    return {
      schIndex: 0,
      schs: [
        {
          name: "sch1",
          addr: 0,
          services: [],
        },
      ],
      showService: false,
      showGroup: false,
      refresh: false,
      sortable: [],
      cd: false,
      cd1: false,
      config: config["uds"],
      group: [],
      editIndex: 0,
      service: {},
      jsFn: [" "],
      codeIndex: 0,
      codeNum: 1,
      codeOption: {
        readOnly: true,
      },
      selectTable: [],
    };
  },
  // use watch to replace
  // destroyed() {
  //   this.$store.commit(this.mode+"TableLoad",this.schs);
  // },
  mounted() {
    if (this.$store.state[this.mode + "Table"].length > 0) {
      this.schs = this.$store.state[this.mode + "Table"];
    }
    this.$nextTick(() => {
      const table = document.querySelectorAll(".el-table__body-wrapper tbody");
      const self = this;
      for (var i = 0; i < table.length; i++) {
        this.sortable[i] = Sortable.create(table[i], {
          group: "g1",
          animation: 150,
          onEnd(val) {
            self.sortableEnd(val);
          },
        });
      }
    });
  },
  props: ["mode"],
  methods: {
    update(){
      if (this.$store.state[this.mode + "Table"].length > 0) {
        this.schs = this.$store.state[this.mode + "Table"];
      }
    },
    sortableEnd(val) {
      var fromIndex = 0;
      var toIndex = 0;
      for (var j = 0; j < this.$refs.srvTable.length; j++) {
        let node = this.$refs.srvTable[j].$el.childNodes[2].childNodes[0]
          .childNodes[1]; //tbody
        if (node.isEqualNode(val.from)) {
          fromIndex = j;
        }
        if (node.isEqualNode(val.to)) {
          toIndex = j;
        }
      }
      const targetRow = this.schs[fromIndex].services.splice(
        val.oldIndex,
        1
      )[0];
      this.schs[toIndex].services.splice(val.newIndex, 0, targetRow);
      // state.canTable.splice(index[0], 0, targetRow)
      // console.log(val.oldIndex,val.newIndex,fromIndex,toIndex);
    },
    deleteSch(val) {
      this.schIndex=0;
      this.schs.splice(val, 1);
      this.sortable.splice(val, 1);
    },
    addSch() {
      this.schs.push({
        name: "sch" + (this.schs.length + 1),
        addr: 0,
        services: [],
      });
      this.$nextTick(() => {
        const self = this;
        const tables = document.querySelectorAll(
          ".el-table__body-wrapper tbody"
        );
        this.sortable.push(
          Sortable.create(tables[tables.length - 1], {
            group: "g1",
            animation: 150,
            onEnd(val) {
              self.sortableEnd(val);
            },
          })
        );
      });
    },
    addService(val) {
      this.showService = true;
      this.schIndex = val;
    },
    addGroup(val) {
      this.showGroup = true;
      this.schIndex = val;
    },
    serviceAddedCb(val) {
      this.showService = false;
      val.date = new Date().getTime();
      this.schs[this.schIndex].services.push(val);
    },
    groupAddedCb() {
      this.showGroup = false;
    },
    editClose(done) {
      var dirty = this.$refs.addService.isDirty();
      if (dirty) {
        this.$confirm("Some changes unsaved, discard?", "Unsaved", {
          confirmButtonText: "Yes",
          cancelButtonText: "No",
          type: "warning",
        })
          .then(() => {
            done();
          })
          .catch(() => {});
      } else {
        done();
      }
    },
    handleSelectionChange(val) {
      this.selectTable = val;
    },
    // eslint-disable-next-line no-unused-vars
    tableRowClassName({ row, rowIndex }) {
      if (rowIndex == this.tableErrorIndex[1] - 1) {
        if (this.schs[this.tableErrorIndex[0]].services.indexOf(row) != -1) {
          return "error";
        }
      }
      return "";
    },
    closeError() {
      this.$store.commit("setTableError", [-1,-1]);
    },
    loadGroup() {
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
    },
    showNewCode(val) {
      this.codeIndex = val - 1;
    },
    showCode(item) {
      this.codeItem = item;
      this.jsFn = [];
      this.codeIndex = 0;
      if (item.type == "uds") {
        this.codeNum = 1;
        this.jsFn[0] = item.func;
      } else {
        for (var i in item.subtable) {
          this.jsFn[i] = item.subtable[i].func;
        }
        this.codeNum = item.subtable.length;
      }

      this.cd = true;
    },
    deleteMulti(schIndex) {
      this.schs[schIndex].services = [];
    },
    deleteService(schIndex, srcIndex) {
      this.schs[schIndex].services.splice(srcIndex, 1);
    },
    editItem(val) {
      this.$set(this.schs[this.editIndex[0]].services, this.editIndex[1], val);
      this.cd1 = false;
    },
    editService(schIndex, srvIndex, val) {
      this.editIndex = [schIndex, srvIndex];
      if (val.type == "uds") {
        for (let i in this.config) {
          if (this.config[i].value == val.service.value) {
            this.cd1 = true;
            this.refresh = false;
            this.service.type = "uds";
            this.service.cfg = this.config[i];
            this.service.val = JSON.parse(JSON.stringify(val));
            this.$nextTick(() => {
              this.refresh = true;
            });
            break;
          }
        }
      } else {
        this.loadGroup();
        for (let i in this.group) {
          if (i == val.service.name) {
            this.refresh = false;
            this.cd1 = true;
            this.service.type = "group";
            this.service.cfg = this.group[i];
            this.service.val = JSON.parse(JSON.stringify(val));
            this.$nextTick(() => {
              this.refresh = true;
            });
            break;
          }
        }
      }
    },
  },
  watch: {
    schs: {
      deep: true,
      handler: function () {
        this.$store.commit(this.mode + "TableLoad", this.schs);
      },
    },
  },
  computed: {
    tableErrorIndex: function () {
      return this.$store.state.tableErrorIndex;
    },
    running: function () {
      return this.$store.state.running;
    },
    addrTable: function () {
      if (this.mode === "doip") {
        return this.$store.state.doipAddrTable;
      } else if (this.mode === "can") {
        return this.$store.state.canAddrTable;
      } else if (this.mode === "lp") {
        return this.$store.state.lpAddrTable;
      } else {
        return [];
      }
    },
  },
};
</script>
<style>
.table_error {
  position: absolute;
  padding: 0px;
  margin: 0px;
  top: 0px;
  right: 0px;
}
.el-table .error {
  background: red;
}
.table_error i {
  z-index: 2;
  position: absolute;
  top: 0px;
  right: 0px;
  font-size: 25px;
  color: #e6a23c;
}
.name {
  font-size: 14px;
  font-weight: bolder;
  margin: 5px;
}
</style>
