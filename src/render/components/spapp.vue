<template>
  <div style="margin: 10px">
    <el-collapse v-model="activeNames">
      <el-collapse-item title="Request Service" name="1">
        <div>
          <el-form
            :inline="true"
            :model="request"
            size="mini"
            :rules="rule1"
            ref="request"
          >
            <el-form-item label="Service:" required prop="service">
              <el-input
                v-model="request.service"
                maxlength="4"
                show-word-limit
                style="width: 150px"
                ><template slot="prepend">0x</template></el-input
              >
            </el-form-item>
            <el-form-item label="Instance:" required prop="instance">
              <el-input
                v-model="request.instance"
                maxlength="4"
                show-word-limit
                style="width: 150px"
                ><template slot="prepend">0x</template></el-input
              >
            </el-form-item>
            <el-form-item>
              <el-button type="primary" @click="sendRequest">Request</el-button>
            </el-form-item>
          </el-form>
          <el-table :data="avlList" style="width: 100%" size="mini">
            <el-table-column prop="service" label="Service">
              <template slot-scope="scope">
                0x{{ scope.row.service.toString(16) }}
              </template>
            </el-table-column>
            <el-table-column prop="instance" label="Instance">
              <template slot-scope="scope">
                0x{{ scope.row.instance.toString(16) }}
              </template>
            </el-table-column>
            <el-table-column prop="avl" label="Avalivale">
              <template slot-scope="scope">
                <span v-if="scope.row.avl">
                  <i class="el-icon-success" style="color: #67c23a"></i>
                </span>
                <span v-else>
                  <i class="el-icon-error" style="color: #f56c6c"></i>
                </span>
              </template>
            </el-table-column>
            <el-table-column label="Action">
              <template slot-scope="scope">
                <el-button @click="deleteReq(scope.$index)" type="text"
                  ><i class="el-icon-delete" style="color: #f56c6c"></i
                ></el-button>
              </template>
            </el-table-column>
          </el-table>
        </div>
      </el-collapse-item>
      <el-collapse-item title="Send Request" name="2">
        <div>
          <el-form
            :model="send"
            size="mini"
            :rules="rule2"
            ref="send"
            label-position="top"
          >
            <el-form-item>
              <el-col :span="12">
                <el-form-item label="Service/Instance" required prop="key">
                  <el-select v-model="send.key">
                    <el-option
                      v-for="(item, index) in avlList"
                      :key="index"
                      :label="`${item.service.toString(
                        16
                      )}-${item.instance.toString(16)}`"
                      :value="index"
                      :disabled="item.avl == 0"
                    >
                    </el-option>
                  </el-select>
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="Method:" required prop="method">
                  <el-input
                    v-model="send.method"
                    maxlength="4"
                    show-word-limit
                    style="width: 150px"
                    ><template slot="prepend">0x</template></el-input
                  >
                </el-form-item>
              </el-col>
            </el-form-item>
            <el-form-item required style="width: 100%">
              <el-input
                style="width: 100%"
                type="textarea"
                :autosize="{ minRows: 2, maxRows: 4 }"
                placeholder="payload"
                v-model="send.payload"
              >
              </el-input>
            </el-form-item>
            <el-form-item>
              <el-col :span="12">
                <el-form-item required>
                  <el-checkbox v-model="send.reliable">Using TCP</el-checkbox>
                </el-form-item>
              </el-col>
              <el-col :span="12" style="text-align: right">
                <el-form-item>
                  <el-button type="primary" @click="sendData">Send</el-button>
                </el-form-item>
              </el-col>
            </el-form-item>
          </el-form>
        </div>
      </el-collapse-item>
      <el-collapse-item title="Offer Service / Set Response" name="3">
        <div>
          <el-form
            label-position="top"
            :model="offer"
            size="mini"
            :rules="rule3"
            ref="offer"
          >
            <el-form-item>
              <el-col :span="8">
                <el-form-item label="Service:" required prop="service">
                  <el-input
                    v-model="offer.service"
                    maxlength="4"
                    show-word-limit
                    style="width: 150px"
                    ><template slot="prepend">0x</template></el-input
                  >
                </el-form-item>
              </el-col>
              <el-col :span="8">
                <el-form-item label="Instance:" required prop="instance">
                  <el-input
                    v-model="offer.instance"
                    maxlength="4"
                    show-word-limit
                    style="width: 150px"
                    ><template slot="prepend">0x</template></el-input
                  >
                </el-form-item>
              </el-col>
              <el-col :span="8">
                <el-form-item label="Method:" required prop="method">
                  <el-input
                    v-model="offer.method"
                    maxlength="4"
                    show-word-limit
                    style="width: 150px"
                    ><template slot="prepend">0x</template></el-input
                  >
                </el-form-item>
              </el-col>
            </el-form-item>
            <el-form-item required style="width: 100%">
              <el-input
                style="width: 100%"
                type="textarea"
                :autosize="{ minRows: 2, maxRows: 4 }"
                placeholder="payload"
                v-model="offer.payload"
              >
              </el-input>
            </el-form-item>
            <el-form-item>
              <el-button type="primary" @click="sendOffer">Set</el-button>
            </el-form-item>
          </el-form>
          <el-table :data="offerList" style="width: 100%" size="mini">
            <el-table-column prop="service" label="Service">
              <template slot-scope="scope">
                0x{{ scope.row.service.toString(16) }}
              </template>
            </el-table-column>
            <el-table-column prop="instance" label="Instance">
              <template slot-scope="scope">
                0x{{ scope.row.instance.toString(16) }}
              </template>
            </el-table-column>
            <el-table-column prop="method" label="Method">
              <template slot-scope="scope">
                0x{{ scope.row.method.toString(16) }}
              </template>
            </el-table-column>
            <el-table-column prop="payload" label="Payload"> </el-table-column>
            <el-table-column label="Action">
              <template slot-scope="scope">
                <el-button @click="deleteOffer(scope.$index)" type="text"
                  ><i class="el-icon-delete" style="color: #f56c6c"></i
                ></el-button>
              </template>
            </el-table-column>
          </el-table>
        </div>
      </el-collapse-item>
    </el-collapse>
  </div>
</template>

<script>
const { ipcRenderer } = require("electron");
export default {
  data() {
    return {
      activeNames: ["1", "2", "3"],
      request: {
        service: "",
        instance: "",
      },
      offer: {
        service: "",
        instance: "",
        method: "",
        payload: "",
      },
      offerList: [],
      send: {
        key: "",
        method: "",
        payload: "",
        reliable: true,
      },
      requestList: [],
      rule1: {
        service: [
          {
            pattern: "^([0-9a-fA-F]{2})+$",
            message: "Please Input HEX Format",
            trigger: "change",
          },
        ],
        instance: [
          {
            pattern: "^([0-9a-fA-F]{2})+$",
            message: "Please Input HEX Format",
            trigger: "change",
          },
        ],
      },
      rule2: {
        method: [
          {
            pattern: "^([0-9a-fA-F]{2})+$",
            message: "Please Input HEX Format",
            trigger: "change",
          },
        ],
        key: [
          {
            required: true,
            message: "Need choose an avaliable requested service",
          },
        ],
        payload: [
          {
            required: true,
            message: "Please input payload",
          },
        ],
      },
      rule3: {
        service: [
          {
            pattern: "^([0-9a-fA-F]{2})+$",
            message: "Please Input HEX Format",
            trigger: "change",
          },
        ],
        instance: [
          {
            pattern: "^([0-9a-fA-F]{2})+$",
            message: "Please Input HEX Format",
            trigger: "change",
          },
        ],
        method: [
          {
            pattern: "^([0-9a-fA-F]{2})+$",
            message: "Please Input HEX Format",
            trigger: "change",
          },
        ],
      },
      avlList: [],
    };
  },
  mounted() {
    ipcRenderer.sendSync("someipStartApp", { name: this.name });
    ipcRenderer.on("someip_avl", (event, val) => {
      let key = `${val.service}-${val.instance}`;
      for (let i in this.avlList) {
        let t = `${this.avlList[i].service}-${this.avlList[i].instance}`;
        if (t == key) {
          this.$set(this.avlList, i, JSON.parse(JSON.stringify(val)));
          return;
        }
      }
      this.avlList.push(JSON.parse(JSON.stringify(val)));
    });
    ipcRenderer.on("someip_msg", (event, val) => {
      let text = `Received:Service:${val.msg.service.toString(
        16
      )},Instance:${val.msg.instance.toString(
        16
      )},Method:${val.msg.method.toString(16)},`;
      text += `Payload:${Buffer.from(val.msg.payload.data).toString(
        "hex"
      )}\r\n`;
      this.terminal.write(`\x1B[1;;36m${text}\x1B[0m`);
    });
  },
  destroyed() {
    ipcRenderer.removeAllListeners("someip_avl");
    ipcRenderer.removeAllListeners("someip_msg");
  },
  methods: {
    deleteReq(index) {
      ipcRenderer.sendSync("someipDeleteRequest", {
        name: this.name,
        service: this.avlList[index].service,
        instance: this.avlList[index].instance,
      });
      this.avlList.splice(index, 1);
    },
    deleteOffer(index) {
      let service = this.offerList[index].service;
      let instance = this.offerList[index].instance;
      let method = this.offerList[index].method;
      this.offerList.splice(index, 1);
      ipcRenderer.sendSync("someipDeleteResponse", {
        name: this.name,
        service: parseInt(service, 16),
        instance: parseInt(instance, 16),
        method: parseInt(method, 16),
      });
      for (let i in this.offerList) {
        if (
          this.offerList[i].service == service &&
          this.offerList[i].instance
        ) {
          return;
        }
      }
      ipcRenderer.sendSync("someipStopOffer", {
        name: this.name,
        service: parseInt(service, 16),
        instance: parseInt(instance, 16),
      });
    },
    sendRequest() {
      this.$refs.request.validate((valid) => {
        if (valid) {
          ipcRenderer.sendSync("someipSendRequest", {
            name: this.name,
            service: parseInt(this.request.service, 16),
            instance: parseInt(this.request.instance, 16),
          });
        } else {
          return false;
        }
      });
    },
    sendOffer() {
      this.$refs.offer.validate((valid) => {
        if (valid) {
          let offered = false;
          let registed = false;
          let key = this.offer.service + "-" + this.offer.instance;
          for (let i in this.offerList) {
            let t =
              this.offerList[i].service + "-" + this.offerList[i].instance;
            if (t == key) {
              offered = true;
              if (this.offerList[i].method == this.offer.method) {
                registed = true;
              }
            }
          }

          if (!offered) {
            ipcRenderer.sendSync("someipOffer", {
              name: this.name,
              service: parseInt(this.offer.service, 16),
              instance: parseInt(this.offer.instance, 16),
            });
          }
          if (!registed) {
            this.offerList.push(JSON.parse(JSON.stringify(this.offer)));
            ipcRenderer.sendSync("someipAddResponse", {
              name: this.name,
              service: parseInt(this.offer.service, 16),
              instance: parseInt(this.offer.instance, 16),
              method: parseInt(this.offer.method, 16),
              payload: this.offer.payload,
            });
          }
        } else {
          return false;
        }
      });
    },
    sendData() {
      this.$refs.send.validate((valid) => {
        if (valid) {
          ipcRenderer.sendSync("someipSend", {
            name: this.name,
            service: this.avlList[this.send.key].service,
            instance: this.avlList[this.send.key].instance,
            method: parseInt(this.send.method, 16),
            reliable: this.send.reliable,
            payload: this.send.payload,
          });
        } else {
          return false;
        }
      });
    },
  },
  props: ["name", "route", "terminal"],
};
</script>

<style>
</style>