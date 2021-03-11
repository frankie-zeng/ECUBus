<template>
  <div>
    <div>
      <el-card shadow="hover">
        <div slot="header">
          <span>HSE SMR</span>
          <el-button
            v-if="activeNames.length > 0"
            style="float: right; padding: 3px 0"
            type="text"
            @click="activeNames = []"
            >Hide</el-button
          >
          <el-button
            v-else
            style="float: right; padding: 3px 0"
            type="text"
            @click="activeNames = ['1', '2']"
            >Show</el-button
          >
        </div>
        <el-collapse v-model="activeNames">
          <el-collapse-item title="Add SMR" name="1">
            <el-alert
              title="SMR install need user upload firmware firstly!"
              type="info"
              :closable="false"
              style="padding: 5px; margin-bottom: 20px"
              v-if="!isFw"
            ></el-alert>
            <el-form
              label-position="left"
              label-width="150px"
              :model="addSmr"
              size="mini"
              ref="smr"
              :rules="rules"
              :disabled="!isFw"
            >
              <el-form-item label="SmrEntry" prop="index" required>
                <el-input-number
                  v-model="addSmr.index"
                  controls-position="right"
                  :min="0"
                  :max="7"
                  @change="indexChange"
                ></el-input-number>
              </el-form-item>
              <el-form-item label="Verify Way" prop="verifyWay" required>
                <el-select
                  v-model="addSmr.verifyWay"
                  placeholder="请选择"
                  style="width: 100%"
                >
                  <el-option
                    label="VERIFY_PRE_BOOT_MASK"
                    :value="0x56"
                  ></el-option>
                  <el-option
                    label="VERIFY_PRE_BOOT_ALT_MASK"
                    :value="0x75"
                  ></el-option>
                  <el-option
                    label="VERIFY_POST_BOOT_MASK"
                    :value="0xa7"
                  ></el-option>
                  <el-option
                    label="VERIFY_PUN_TIME_MASK"
                    :value="0x65"
                    disabled
                  ></el-option>
                </el-select>
              </el-form-item>
              <el-form-item label="StartAddr" prop="startAddr" required>
                <el-input
                  v-model="addSmr.startAddr"
                  :maxlength="8"
                  show-word-limit
                >
                  <template slot="prepend">0x</template></el-input
                >
              </el-form-item>
              <el-form-item label="Length" prop="length" required>
                <el-input v-model="addSmr.length"
                  ><template slot="prepend">0x</template></el-input
                >
              </el-form-item>
              <el-form-item label="Target Key" prop="key" required>
                <el-select
                  v-model="addSmr.key"
                  placeholder="请选择"
                  style="width: 100%"
                  filterable
                  @change="targetKeyChange"
                >
                  <el-option
                    v-for="(item, index) in keyTable"
                    :key="index"
                    :label="item.label"
                    :value="item.handle"
                    :disabled="item.handle == 0x10001 && addSmr.index != 0"
                  >
                    <span style="float: left">{{ item.label }}</span>
                    <span
                      v-if="item.keyLoaded"
                      style="float: right; color: green; font-size: 13px"
                      ><i class="el-icon-success"></i
                    ></span>
                    <span
                      v-else
                      style="float: right; color: #e6a23c; font-size: 13px"
                      ><i class="el-icon-warning"></i
                    ></span>
                  </el-option>
                </el-select>
              </el-form-item>
              <el-form-item label="Flash Proof" prop="proof" required>
                <el-checkbox v-model="addSmr.proof" :disabled="mac"
                  >Proof In Flash</el-checkbox
                >
              </el-form-item>
              <el-form-item
                label="ProofAddr"
                prop="proofAddr"
                :required="addSmr.proof"
              >
                <el-input
                  v-model="addSmr.proofAddr"
                  :maxlength="8"
                  show-word-limit
                  :disabled="mac"
                  ><template slot="prepend">0x</template></el-input
                >
              </el-form-item>
              <el-form-item label="Sign Key" prop="signKey" required>
                <el-input
                  v-if="mac"
                  :maxlength="keySize / 4"
                  v-model="addSmr.signKey"
                  show-word-limit
                >
                  <template slot="prepend">0x</template>
                </el-input>
                <el-input
                  v-else
                  v-model="addSmr.signKey"
                  type="textarea"
                  :autosize="{ minRows: 1, maxRows: 12 }"
                ></el-input>
              </el-form-item>
              <el-form-item label="Auth Scheme" prop="authWay" required>
                <el-select
                  v-model="addSmr.authWay"
                  placeholder="请选择"
                  style="width: 100%"
                  filterable
                  @change="schemeChange"
                >
                  <el-option
                    v-for="item in signTable"
                    :key="item.value"
                    :label="item.value"
                    :value="item.value"
                    :disabled="!item.support"
                  >
                  </el-option>
                </el-select>
              </el-form-item>
              <el-form-item
                label="IV"
                prop="iv"
                :required="addSmr.authWay == 'gmac'"
              >
                <el-input :maxlength="24" v-model="addSmr.iv" show-word-limit>
                  <template slot="prepend">0x</template>
                </el-input>
              </el-form-item>
              <el-form-item>
                <el-button type="primary" @click="addSmrHandle">Add</el-button>
              </el-form-item>
            </el-form>
          </el-collapse-item>
          <el-collapse-item title="SMR Table" name="2">
            <el-table
              :data="smrTable"
              style="width: 100%"
              size="mini"
              ref="ram"
              row-key="date"
            >
              <!-- <el-table-column width="180" label="Group Num">
                <template slot-scope="scope">
                  {{ scope.$index }}
                </template>
              </el-table-column> -->
              <el-table-column prop="index" label="SMR Entry" width="180">
              </el-table-column>
              <el-table-column
                prop="startAddr"
                label="Start Address"
                width="180"
              >
                <template slot-scope="scope">
                  {{ "0x" + scope.row.startAddr.toString(16) }}
                </template>
              </el-table-column>
              <el-table-column prop="length" label="Length" width="180">
                <template slot-scope="scope">
                  {{ "0x" + scope.row.length.toString(16) }}
                </template>
              </el-table-column>
              <el-table-column prop="key" label="Target Key" width="180">
                <template slot-scope="scope">
                  {{ "0x" + scope.row.key.toString(16) }}
                </template>
              </el-table-column>
              <el-table-column prop="verifyWay" label="Verify Way">
                <template slot-scope="scope">
                  <span v-if="scope.row.verifyWay == 0x56"
                    >VERIFY_PRE_BOOT_MASK</span
                  >
                  <span v-else-if="scope.row.verifyWay == 0x75"
                    >VERIFY_PRE_BOOT_ALT_MASK</span
                  >
                  <span v-else-if="scope.row.verifyWay == 0xa7"
                    >VERIFY_POST_BOOT_MASK</span
                  >
                  <span v-else>VERIFY_RUN_TIME_MASK</span>
                </template>
              </el-table-column>
              <el-table-column
                prop="proof"
                label="Flash In Proof"
                width="180"
                align="center"
              >
                <template slot-scope="scope">
                  <i class="el-icon-circle-check" v-if="scope.row.proof"></i>
                  <i class="el-icon-circle-close" v-else></i>
                </template>
              </el-table-column>
              <el-table-column prop="proofAddr" label="Proof Address">
                <template slot-scope="scope">
                  {{ "0x" + scope.row.proofAddr.toString(16) }}
                </template>
              </el-table-column>
              <el-table-column prop="tag" label="Auth Tag">
                <template slot-scope="scope">
                  {{ scope.row.tag.toString(16) }}
                </template>
              </el-table-column>

              <el-table-column
                fixed="right"
                label="Action"
                width="150"
                align="center"
              >
                <template slot-scope="scope">
                  <!-- <h1 v-if="scope.$index==tableErrorIndex">error</h1> -->
                  <el-button
                    type="danger"
                    size="mini"
                    icon="el-icon-delete"
                    circle
                    @click="deleteSmr(scope.$index)"
                  ></el-button>
                </template>
              </el-table-column>
            </el-table>
          </el-collapse-item>
        </el-collapse>
      </el-card>
    </div>
  </div>
</template>

<script>
const { ipcRenderer } = require("electron");
export default {
  data() {
    return {
      mac: true,
      activeNames: ["1", "2"],
      smrTable: [],
      keyType: "",
      keySize: "",
      signTable: [],
      rules: {
        startAddr: [
          {
            required: true,
            pattern: /^[0-9a-fA-F]+$/,
            message: 'Please input HEX format，eg:"0001020304"',
            trigger: "change",
          },
        ],
        proofAddr: [
          {
            pattern: /^[0-9a-fA-F]+$/,
            message: 'Please input HEX format，eg:"0001020304"',
            trigger: "change",
          },
        ],
        iv: [
          {
            pattern: /^[0-9a-fA-F]+$/,
            message: 'Please input HEX format，eg:"0001020304"',
            trigger: "change",
          },
        ],
      },
      addSmr: {
        index: 0,
        startAddr: "400000",
        verifyWay: 0x56,
        length: "",
        proof: false,
        proofAddr: "",
        key: "",
        signKey: "",
        iv: "",
        authWay: "",
        proofValue: "",
      },
    };
  },
  props: {
    isFw: {
      type: Boolean,
      default: function () {
        return false;
      },
    },
    fwInfo: {
      type: Object,
      default: function () {
        return {};
      },
    },
  },
  mounted() {
    this.smrTable = this.$store.state.hseConfig.smr;
  },
  methods: {
    addSmrHandle() {
      this.$refs.smr.validate((valid) => {
        if (valid) {
          /*verify address*/
          let clone = JSON.parse(JSON.stringify(this.addSmr));
          clone.startAddr = parseInt(clone.startAddr, 16);
          clone.proofAddr = parseInt(clone.proofAddr, 16);
          clone.length = parseInt(clone.length, 16);
          clone.mac = this.mac;
          console.log(clone);
          if (clone.startAddr < this.fwInfo.startAddr) {
            this.$notify.error({
              title: "Error",
              message:
                "Start address should large than firmware start address!",
              duration: 0,
            });
            return false;
          }

          if (
            clone.startAddr + clone.length >
            this.fwInfo.startAddr + this.fwInfo.size
          ) {
            this.$notify.error({
              title: "Error",
              message: "Length is large than firmware size!",
              duration: 0,
            });
            return false;
          }

          if (clone.proof) {
            if (
              clone.proofAddr < this.fwInfo.startAddr ||
              clone.proofAddr > this.fwInfo.startAddr + this.fwInfo.size
            ) {
              this.$notify.error({
                title: "Error",
                message: "Proof address should in firmware!",
                duration: 0,
              });
              return false;
            }
          }
          //calculate tag
          let ret = ipcRenderer.sendSync("signFw", {
            file: this.fwInfo.fileName,
            info: clone,
          });
          if (!ret.err) {
            this.$notify.error({
              title: "Error",
              message: ret.msg,
              duration: 0,
            });
            return false;
          }
          clone.tag = ret.tag;

          for (let i = 0; i < this.smrTable.length; i++) {
            if (this.smrTable[i].index == clone.index) {
              //same smr index
              this.$set(this.smrTable, i, clone);
              return;
            }
          }
          this.smrTable.push(clone);
        } else {
          return false;
        }
      });
    },
    deleteSmr(index) {
      this.smrTable.splice(index, 1);
    },
    schemeChange() {},
    indexChange() {
      this.addSmr.key = "";
      this.targetKeyChange();
    },
    targetKeyChange() {
      this.addSmr.authWay = "";
      this.addSmr.proof = false;
      this.addSmr.proofAddr = "";
      this.addSmr.signKey = "";
      this.$nextTick(() => {
        this.$refs.smr.clearValidate();
      });
      let gorup = (this.addSmr.key >> 8) & 0xff;
      let slot = this.addSmr.key & 0xff;
      this.keySize = this.$store.state.hseConfig.format.nvm[gorup].keySize;
      this.keyType = this.$store.state.hseConfig.format.nvm[gorup].keyType;
      const symKeyType = [0x11, 0x12, 0x20, 0x30];
      const privKeyType = [0x87, 0x97, 0xa7];
      this.mac = symKeyType.indexOf(parseInt(this.keyType)) != -1;

      let priv = privKeyType.indexOf(parseInt(this.keyType)) != -1;
      if (
        this.$store.state.hseConfig.format.nvm[gorup].keyHandle[slot]
          .keyLoaded &&
        (this.mac || priv)
      ) {
        if (this.keyType == 0x11) {
          this.addSmr.signKey = this.$store.state.hseConfig.format.nvm[
            gorup
          ].keyHandle[slot].value.raw.keyValue;
        } else {
          this.addSmr.signKey = this.$store.state.hseConfig.format.nvm[
            gorup
          ].keyHandle[slot].value.data;
        }
      }
      if (!this.mac) {
        if (this.keyType == 0x87 || this.keyType == 0x88) {
          this.$set(this, "signTable", [
            {
              value: "ecdsa-sha224",
              support: true,
            },
            {
              value: "ecdsa-sha256",
              support: true,
            },
            {
              value: "ecdsa-sha384",
              support: true,
            },
            {
              value: "ecdsa-sha512",
              support: true,
            },
            {
              value: "eddsa",
              support: true,
            },
          ]);
        } else {
          this.$set(this, "signTable", [
            {
              value: "rsa-pkcs1-sha2-224",
              support: true,
            },
            {
              value: "rsa-pkcs1-sha2-256",
              support: true,
            },
            {
              value: "rsa-pkcs1-sha2-384",
              support: true,
            },
            {
              value: "rsa-pkcs1-sha2-512",
              support: true,
            },
            {
              value: "rsa-pss-sha2-224",
              support: true,
            },
            {
              value: "rsa-pss-sha2-256",
              support: true,
            },
            {
              value: "rsa-pss-sha2-384",
              support: true,
            },
            {
              value: "rsa-pss-sha2-512",
              support: true,
            },
          ]);
        }
      } else {
        if (this.keyType == 0x11) {
          this.$set(this, "signTable", [
            {
              value: "cmac-aes",
              support: true,
            },
          ]);
        } else {
          this.$set(this, "signTable", [
            {
              value: "cmac-aes",
              support: true,
            },
            {
              value: "cmac-tdes",
              support: true,
            },
            {
              value: "gmac",
              support: true,
            },
            {
              value: "hmac-sha2-224",
              support: true,
            },
            {
              value: "hmac-sha2-256",
              support: true,
            },
            {
              value: "hmac-sha2-384",
              support: true,
            },
            {
              value: "hmac-sha2-512",
              support: true,
            },
          ]);
        }
      }
    },
  },
  // watch: {
  //   smrTable: function (val) {
  //     this.$store.commit("hseSmrLoad", val);
  //   },
  // },
  computed: {
    keyTable: function () {
      let nvm = this.$store.state.hseConfig.format.nvm;
      let table = [];
      for (let i = 0; i < nvm.length; i++) {
        if (
          nvm[i].keyType == 0xa7 ||
          nvm[i].keyType == 0xa8 ||
          nvm[i].keyType == 0x89 ||
          nvm[i].keyType == 0x99
        ) {
          //dh type key can't verify, ext type key can't verify;
          continue;
        }
        let group = nvm[i].keyHandle;
        for (let j = 0; j < group.length; j++) {
          //TODO:check keyload status
          // if(gorup[j].keyLoaded){
          //   //check verify flag in
          // }
          if (nvm[i].keyType == 0x11) {
            if (group[j].handle != 0x10001) {
              //she only boot mac key can as sign key;
              continue;
            }
          }
          table.push(group[j]);
        }
      }
      return table;
    },
  },
};
</script>

<style>
</style>