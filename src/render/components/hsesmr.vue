<template>
  <div>
    <div>
      <el-card shadow="hover">
        <div slot="header">
          <span>HSE Format</span>
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
                ></el-input-number>
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
                <el-checkbox v-model="addSmr.proof">Proof In Flash</el-checkbox>
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
                  ><template slot="prepend">0x</template></el-input
                >
              </el-form-item>
              <el-form-item label="Sign Key" prop="signKey" required>
                <el-input
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
export default {
  data() {
    return {
      symKeyType: [0x12, 0x20, 0x30],
      activeNames: ["1"],
      smrTable: [],
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
      },
      addSmr: {
        index: 0,
        startAddr: "400000",
        length: "",
        proof: false,
        proofAddr: "",
        key: "",
        signKey: "",
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
    targetKeyChange() {
      this.addSmr.authWay = "";
    },
  },
  watch: {
    watch: {
      smrTable: function (val) {
        this.$store.commit("hseSmrLoad", val);
      },
    },
  },
  computed: {
    keyTable: function () {
      let nvm = this.$store.state.hseConfig.format.nvm;
      let table = [];
      for (let i = 0; i < nvm.length; i++) {
        let group = nvm[i].keyHandle;
        for (let j = 0; j < group.length; j++) {
          table.push(group[j]);
        }
      }
      return table;
    },
    keyType: function () {
      if (this.addSmr.key != "") {
        let gorup = (this.addSmr.key >> 8) & 0xff;
        return this.$store.state.hseConfig.format.nvm[gorup].keyType;
      } else {
        return 0;
      }
    },
    signTable: function () {
      if (this.symKeyType.indexOf(parseInt(this.keyType)) == -1) {
        return [
          {
            value: "rsa-pkcs1-sha2-224",
            support: false,
          },
          {
            value: "rsa-pkcs1-sha2-256",
            support: true,
          },
          {
            value: "rsa-pkcs1-sha2-384",
            support: false,
          },
          {
            value: "rsa-pkcs1-sha2-512",
            support: false,
          },
          {
            value: "rsa-pss-sha2-224",
            support: false,
          },
          {
            value: "rsa-pss-sha2-256",
            support: false,
          },
          {
            value: "rsa-pss-sha2-384",
            support: false,
          },
          {
            value: "rsa-pss-sha2-512",
            support: false,
          },
        ];
      } else {
        return [
          {
            value: "cmac-aes",
            support: true,
          },
          {
            value: "cmac-tdes",
            support: false,
          },
          {
            value: "gmac",
            support: false,
          },
          {
            value: "hmac-sha2-224",
            support: false,
          },
          {
            value: "hmac-sha2-256",
            support: false,
          },
          {
            value: "hmac-sha2-384",
            support: false,
          },
          {
            value: "hmac-sha2-512",
            support: false,
          },
        ];
      }
    },
  },
};
</script>

<style>
</style>