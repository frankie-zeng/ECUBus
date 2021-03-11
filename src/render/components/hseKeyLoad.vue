<template>
  <div>
    <h3>{{ keyTypeMap[group.keyType] }}</h3>
    <div v-if="group.keyType == 0x11">
      <!-- SHE Key Load -->
      <el-form
        :model="sheKeyLoad"
        :rules="sheRules"
        ref="sheForm"
        label-width="120px"
        label-position="right"
        size="mini"
      >
        <el-form-item label="Auth Key ID:" prop="authKeyId">
          <el-select
            v-model="sheKeyLoad.authKeyId"
            placeholder="Auth Key"
            style="width: 100%"
            filterable
          >
            <el-option
              v-for="item in allKeyHandle"
              :key="item.handle"
              :label="item.label + ' (' + sheSpecMap[item.handle].label + ')'"
              :value="item.handle"
            >
            </el-option>
          </el-select>
        </el-form-item>
        <el-form-item label="Auth Key Vaue:" prop="authKeyValue">
          <el-input
            v-model="sheKeyLoad.authKeyValue"
            placeholder="hex"
            maxlength="32"
            show-word-limit
          ></el-input>
        </el-form-item>
        <el-form-item label="New Key ID:" prop="keyId">
          <el-select
            v-model="sheKeyLoad.keyId"
            placeholder="请选择"
            style="width: 100%"
            filterable
          >
            <el-option
              v-for="item in keyHandle"
              :key="item.handle"
              :label="item.label + ' (' + sheSpecMap[item.handle].label + ')'"
              :value="item.handle"
            >
            </el-option>
          </el-select>
        </el-form-item>
        <el-form-item label="New Key Vaue:" prop="keyValue">
          <el-input
            v-model="sheKeyLoad.keyValue"
            placeholder="hex"
            maxlength="32"
            show-word-limit
          ></el-input>
        </el-form-item>
        <el-form-item label="Flags:" prop="flag">
          <el-checkbox-group v-model="sheKeyLoad.flag">
            <el-checkbox label="WRITE_PROT" name="type"></el-checkbox>
            <el-checkbox label="BOOT_PROT" name="type"></el-checkbox>
            <el-checkbox label="DEBUG_PROT" name="type"></el-checkbox>
            <el-checkbox label="KEY_USAGE" name="type"></el-checkbox>
            <el-checkbox label="WILD_CARD" name="type"></el-checkbox>
            <el-checkbox label="VERIFY_ONLY" name="type"></el-checkbox>
          </el-checkbox-group>
        </el-form-item>
        <el-form-item label="UID:" prop="uid">
          <el-input
            v-model="sheKeyLoad.uid"
            placeholder="hex"
            maxlength="16"
            show-word-limit
          ></el-input>
        </el-form-item>
        <el-form-item label="Counter:" prop="counter">
          <el-input-number
            v-model="sheKeyLoad.counter"
            controls-position="right"
            :min="1"
          ></el-input-number>
        </el-form-item>
        <el-form-item style="text-align: right">
          <el-button type="primary" @click="loadSheKey()">Load</el-button>
        </el-form-item>
      </el-form>
    </div>
    <div v-else>
      <el-form
        :model="genKeyLoad"
        :rules="genRules"
        ref="genForm"
        label-width="120px"
        label-position="right"
        size="mini"
      >
        <el-form-item label="Key" prop="key" required>
          <el-select
            v-model="genKeyLoad.key"
            placeholder="请选择"
            style="width: 100%"
            filterable
          >
            <el-option
              v-for="item in keyHandle"
              :key="item.handle"
              :label="item.label"
              :value="item.handle"
            >
            </el-option>
          </el-select>
        </el-form-item>
        <el-form-item label="SMR Mask" prop="smrMask">
          <el-input
            v-model="genKeyLoad.smrMask"
            placeholder="hex"
            maxlength="2"
            show-word-limit
          ></el-input>
        </el-form-item>
        <el-form-item label="Usage Flags:" prop="flags">
          <el-checkbox-group v-model="genKeyLoad.flags" @change="flagChange">
            <el-checkbox label="WRITE_PROT" name="type"></el-checkbox>
            <el-checkbox label="DEBUG_PROT" name="type"></el-checkbox>
            <el-checkbox label="EXPORTABLE" name="type"></el-checkbox>
            <el-checkbox
              label="ENCRYPTO"
              name="type"
              :disabled="!keyRightMap[group.keyType][0]"
            ></el-checkbox>
            <el-checkbox
              label="DECRYPT"
              name="type"
              :disabled="!keyRightMap[group.keyType][1]"
            ></el-checkbox>
            <el-checkbox
              label="SIGN"
              name="type"
              :disabled="!keyRightMap[group.keyType][2]"
            ></el-checkbox>
            <el-checkbox
              label="VERIFY"
              name="type"
              :disabled="!keyRightMap[group.keyType][3]"
            ></el-checkbox>
            <el-checkbox
              label="EXCHANGE"
              name="type"
              :disabled="!keyRightMap[group.keyType][4]"
            ></el-checkbox>
            <el-checkbox
              label="DERIVE"
              name="type"
              :disabled="!keyRightMap[group.keyType][5]"
            ></el-checkbox>
            <el-checkbox
              label="PROVISION"
              name="type"
              :disabled="!keyRightMap[group.keyType][6]"
            ></el-checkbox>
            <el-checkbox
              label="AUTHORIZATION"
              name="type"
              :disabled="
                !keyRightMap[group.keyType][3] ||
                genKeyLoad.flags.indexOf('SIGN') != -1
              "
            ></el-checkbox>
          </el-checkbox-group>
        </el-form-item>
        <el-form-item label="Counter" prop="counter">
          <el-input-number
            v-model="genKeyLoad.counter"
            controls-position="right"
            :min="1"
          ></el-input-number>
        </el-form-item>
        <el-form-item label="Value" prop="value">
          <el-input
            v-if="
              group.keyType == 0x12 ||
              group.keyType == 0x20 ||
              group.keyType == 0x30
            "
            v-model="genKeyLoad.value"
            placeholder="hex"
            :maxlength="group.keySize / 4"
            show-word-limit
          ></el-input>
          <el-input
            v-else
            type="textarea"
            :autosize="{ minRows: 4, maxRows: 12 }"
            v-model="genKeyLoad.value"
            placeholder="PEM"
            show-word-limit
          ></el-input>
        </el-form-item>
        <!-- <el-divider>Crypto Key Import?</el-divider>
        <el-form-item label="Crypto Key" prop="cryptoKey">
          <el-select
            v-model="genKeyLoad.cryptoKey"
            placeholder="请选择"
            style="width: 100%"
            filterable
            @change="cyptoKeyChange"
          >
            <el-option
              v-for="item in cryptoKeyHandle"
              :key="item.handle"
              :label="item.label"
              :value="item.handle"
            >
            </el-option>
          </el-select>
        </el-form-item>
        <el-form-item label="Crypto Way" prop="cryptoWay">
          <el-select
            v-model="genKeyLoad.cryptoWay"
            placeholder="请选择"
            style="width: 100%"
            filterable
          >
            <el-option
              v-for="item in cryptoWayMap"
              :key="item"
              :label="item"
              :value="item"
            >
            </el-option>
          </el-select>
        </el-form-item> -->
        <el-form-item style="text-align: right">
          <el-button type="primary" @click="loadGenKey()">Load</el-button>
        </el-form-item>
      </el-form>
    </div>
    <el-divider></el-divider>
    <el-table
      :data="keyHandle"
      style="width: 100%"
      size="mini"
      ref="nvm"
      row-key="handle"
    >
      <el-table-column width="180" label="Slot Number">
        <template slot-scope="scope">
          {{ scope.$index }}
        </template>
      </el-table-column>
      <el-table-column prop="label" label="Name"> </el-table-column>
      <el-table-column prop="handle" label="Key Handle" width="180">
        <template slot-scope="scope">
          {{ "0x" + scope.row.handle.toString(16) }}
        </template>
      </el-table-column>
      <el-table-column prop="value" label="Key Value">
        <template slot-scope="scope">
          <div v-if="!scope.row.keyLoaded">NULL</div>
          <div v-else>
            <div v-if="group.keyType == 0x11">
              <div>M1:{{ scope.row.value.data.M1 }}</div>
              <div>M2:{{ scope.row.value.data.M2 }}</div>
              <div>M3:{{ scope.row.value.data.M3 }}</div>
            </div>
            <div v-else-if="symKeyType.indexOf(parseInt(group.keyType)) != -1">
              0x{{ scope.row.value.data }}
            </div>
            <div v-else>
              {{ scope.row.value.data }}
            </div>
          </div>
        </template>
      </el-table-column>
    </el-table>
  </div>
</template>

<script>
const { ipcRenderer } = require("electron");

const keyTypeMap = {
  0x11: "HSE_KEY_TYPE_SHE",
  0x12: "HSE_KEY_TYPE_AES",
  0x20: "HSE_KEY_TYPE_HMAC",
  0x30: "HSE_KEY_TYPE_TDES",
  0x40: "HSE_KEY_TYPE_SHARED_SECRET",
  0x87: "HSE_KEY_TYPE_ECC_PAIR",
  0x88: "HSE_KEY_TYPE_ECC_PUB",
  0x89: "HSE_KEY_TYPE_ECC_PUB_EXT",
  0x97: "HSE_KEY_TYPE_RSA_PAIR",
  0x98: "HSE_KEY_TYPE_RSA_PUB",
  0x99: "HSE_KEY_TYPE_RSA_PUB_EXT",
  0xa7: "HSE_KEY_TYPE_DH_PAIR",
  0xa8: "HSE_KEY_TYPE_DH_PUB",
};
const keyRightMap = {
  0x12: [true, true, true, true, false, true, true],
  0x20: [false, false, true, true, false, true, true],
  0x30: [true, true, true, true, false, true, true],
  0x40: [true, true, true, true, false, true, false],
  0x87: [false, false, true, true, true, false, true],
  0x88: [false, false, false, true, true, false, true],
  0x89: [false, false, false, true, true, false, true],
  0x97: [true, true, true, true, false, false, true],
  0x98: [true, false, false, true, false, false, true],
  0x99: [true, false, false, true, false, false, true],
  0xa7: [false, false, false, false, true, false, false],
  0xa8: [false, false, false, false, true, false, false],
};

const sheSpecMap = {
  0x10000: {
    label: "MASTER_ECU_KEY",
    value: [0, 1],
  },
  0x10001: {
    label: "BOOT_MAC_KEY",
    value: [0, 2],
  },
  0x10002: {
    label: "BOOT_MAC",
    value: [0, 3],
  },
  0x10003: {
    label: "KEY1",
    value: [0, 4],
  },
  0x10004: {
    label: "KEY2",
    value: [0, 5],
  },
  0x10005: {
    label: "KEY3",
    value: [0, 6],
  },
  0x10006: {
    label: "KEY4",
    value: [0, 7],
  },
  0x10007: {
    label: "KEY5",
    value: [0, 8],
  },
  0x10008: {
    label: "KEY6",
    value: [0, 9],
  },
  0x10009: {
    label: "KEY7",
    value: [0, 10],
  },
  0x1000a: {
    label: "KEY8",
    value: [0, 11],
  },
  0x1000b: {
    label: "KEY9",
    value: [0, 12],
  },
  0x1000c: {
    label: "KEY10",
    value: [0, 13],
  },
  0x10100: {
    label: "KEY11",
    value: [1, 4],
  },
  0x10101: {
    label: "KEY12",
    value: [1, 5],
  },
  0x10102: {
    label: "KEY13",
    value: [1, 6],
  },
  0x10103: {
    label: "KEY14",
    value: [1, 7],
  },
  0x10104: {
    label: "KEY15",
    value: [1, 8],
  },
  0x10105: {
    label: "KEY16",
    value: [1, 9],
  },
  0x10106: {
    label: "KEY17",
    value: [1, 10],
  },
  0x10107: {
    label: "KEY18",
    value: [1, 11],
  },
  0x10108: {
    label: "KEY19",
    value: [1, 12],
  },
  0x10109: {
    label: "KEY20",
    value: [1, 13],
  },
  0x10200: {
    label: "KEY21",
    value: [2, 4],
  },
  0x10201: {
    label: "KEY22",
    value: [2, 5],
  },
  0x10202: {
    label: "KEY23",
    value: [2, 6],
  },
  0x10203: {
    label: "KEY24",
    value: [2, 7],
  },
  0x10204: {
    label: "KEY25",
    value: [2, 8],
  },
  0x10205: {
    label: "KEY26",
    value: [2, 9],
  },
  0x10206: {
    label: "KEY27",
    value: [2, 10],
  },
  0x10207: {
    label: "KEY28",
    value: [2, 11],
  },
  0x10208: {
    label: "KEY29",
    value: [2, 12],
  },
  0x10209: {
    label: "KEY30",
    value: [2, 13],
  },
  0x10300: {
    label: "KEY31",
    value: [3, 4],
  },
  0x10301: {
    label: "KEY32",
    value: [3, 5],
  },
  0x10302: {
    label: "KEY33",
    value: [3, 6],
  },
  0x10303: {
    label: "KEY34",
    value: [3, 7],
  },
  0x10304: {
    label: "KEY35",
    value: [3, 8],
  },
  0x10305: {
    label: "KEY36",
    value: [3, 9],
  },
  0x10306: {
    label: "KEY37",
    value: [3, 10],
  },
  0x10307: {
    label: "KEY38",
    value: [3, 11],
  },
  0x10308: {
    label: "KEY39",
    value: [3, 12],
  },
  0x10309: {
    label: "KEY40",
    value: [3, 13],
  },
  0x10400: {
    label: "KEY41",
    value: [4, 4],
  },
  0x10401: {
    label: "KEY42",
    value: [4, 5],
  },
  0x10402: {
    label: "KEY43",
    value: [4, 6],
  },
  0x10403: {
    label: "KEY44",
    value: [4, 7],
  },
  0x10404: {
    label: "KEY45",
    value: [4, 8],
  },
  0x10405: {
    label: "KEY46",
    value: [4, 9],
  },
  0x10406: {
    label: "KEY47",
    value: [4, 10],
  },
  0x10407: {
    label: "KEY48",
    value: [4, 11],
  },
  0x10408: {
    label: "KEY49",
    value: [4, 12],
  },
  0x10409: {
    label: "KEY50",
    value: [4, 13],
  },
};
export default {
  data() {
    return {
      symKeyType: [0x12, 0x20, 0x30],
      pubKeyType: [0x88, 0x98, 0xa8],
      prvKeyType: [0x87, 0x97, 0xa7],
      keyRightMap: keyRightMap,
      sheSpecMap: sheSpecMap,
      keyTypeMap: keyTypeMap,
      keyHandle: [],
      allKeyHandle: [],
      cryptoKeyHandle: [],
      sheKeyLoad: {
        authKeyId: "",
        authKeyValue: "492c29292b031f4571a870207c47056e",
        keyId: "",
        keyValue: "492c29292b031f4571a870207c47056e",
        flag: [],
        counter: 1,
        uid: "4571a870207c4705",
      },
      sheRules: {
        authKeyId: [
          {
            required: true,
            message: "请输入用于验证的KEY-ID",
            trigger: "blur",
          },
        ],
        authKeyValue: [
          {
            required: true,
            pattern: /^[0-9a-fA-F]+$/,
            message: '请输入用于验证的KEY值，eg:"0001020304"',
            trigger: "change",
          },
          { min: 32, max: 32, message: "KEY的长度为16个字节", trigger: "blur" },
        ],
        keyId: [
          {
            required: true,
            message: "请输入用于验证的KEY-ID",
            trigger: "blur",
          },
        ],
        keyValue: [
          {
            required: true,
            pattern: /^[0-9a-fA-F]+$/,
            message: '请输入用于验证的KEY值，HEX格式的形式，eg:"0001020304"',
            trigger: "change",
          },
          { min: 32, max: 32, message: "KEY的长度为16个字节", trigger: "blur" },
        ],
        uid: [
          {
            required: true,
            pattern: /^[0-9a-fA-F]+$/,
            message: '请输入用于验证的KEY值，eg:"0001020304"',
            trigger: "change",
          },
          { min: 16, max: 16, message: "UID的长度为8个字节", trigger: "blur" },
        ],
      },
      cryptoWayMap: [],
      genKeyLoad: {
        key: "",
        flags: [],
        counter: 1,
        smrMask: 0,
        value: "",
        cryptoKey: "",
        cryptoWay: "",
      },
      genRules: {},
    };
  },
  mounted() {
    this.keyHandle = this.group.keyHandle;
    this.allKeyHandle = [];
    this.cryptoKeyHandle = [];
    let nvm = this.$store.state.hseConfig.format.nvm;
    if (this.group.keyType == 0x11) {
      for (let i = 0; i < nvm.length; i++) {
        if (nvm[i].keyType == this.group.keyType) {
          this.allKeyHandle = this.allKeyHandle.concat(nvm[i].keyHandle);
        }
      }
    } else {
      for (let i = 0; i < nvm.length; i++) {
        if (
          nvm[i].keyType == 0x12 ||
          nvm[i].keyType == 0x30 ||
          nvm[i].keyType == 0x97
        ) {
          if (nvm[i].keyOwn == this.group.keyOwn) {
            this.cryptoKeyHandle = this.cryptoKeyHandle.concat(
              nvm[i].keyHandle
            );
          }
        }
      }
    }
  },
  props: {
    group: {
      type: Object,
      default: function () {
        return {};
      },
    },
  },
  methods: {
    flagChange() {
      let index1 = this.genKeyLoad.flags.indexOf("SIGN");
      let index2 = this.genKeyLoad.flags.indexOf("AUTHORIZATION");
      if (index2 != -1 && index1 != -1) {
        this.genKeyLoad.flags.splice(index2, 1);
      }
    },
    loadSheKey() {
      this.$refs.sheForm.validate((valid) => {
        if (valid) {
          let she = JSON.parse(JSON.stringify(this.sheKeyLoad));
          she.keyId = this.sheSpecMap[she.keyId].value;
          she.authKeyId = this.sheSpecMap[she.authKeyId].value;
          let M = ipcRenderer.sendSync("k3SHEGen", she);
          this.keyHandle[this.sheKeyLoad.keyId & 0xff].value = {
            raw: this.sheKeyLoad,
            data: M,
            type: "she",
          };
          this.keyHandle[this.sheKeyLoad.keyId & 0xff].keyLoaded = true;
        } else {
          return false;
        }
      });
    },
    loadGenKey() {
      //jsut use crypto module way to verify key format
      this.$refs.genForm.validate((valid) => {
        if (valid) {
          let keyType = parseInt(this.group.keyType);
          let ret;
          if (this.symKeyType.indexOf(keyType) != -1) {
            ret = ipcRenderer.sendSync("importKey", {
              way: "secret",
              value: this.genKeyLoad.value,
            });
          } else if (this.pubKeyType.indexOf(keyType) != -1) {
            ret = ipcRenderer.sendSync("importKey", {
              way: "public",
              value: this.genKeyLoad.value,
            });
          } else if (this.prvKeyType.indexOf(keyType) != -1) {
            ret = ipcRenderer.sendSync("importKey", {
              way: "private",
              value: this.genKeyLoad.value,
            });
          } else {
            return false;
          }
          if (ret.err) {
            this.keyHandle[this.genKeyLoad.key & 0xff].value = {
              raw: this.sheKeyLoad,
              data: this.genKeyLoad.value,
              type: "gen",
            };
            this.keyHandle[this.genKeyLoad.key & 0xff].keyLoaded = true;
          } else {
            this.$notify.error({
              title: "Error",
              message: ret.msg,
              duration: 0,
            });
            return false;
          }
        } else {
          return false;
        }
      });
    },
    cyptoKeyChange() {
      let group = (this.genKeyLoad.cryptoKey >> 8) & 0xff;
      let keyType = this.$store.state.hseConfig.format.nvm[group].keyType;
      this.cryptoWayMap = [];
      if (keyType == 0x12) {
        //aes key
        this.cryptoWayMap = [
          "aes-cbc",
          "aes-ctr",
          "aes-ecb",
          "aes-cfb",
          "aes-ofb",
          "aes-gcm",
          "aes-ccm",
        ];
      } else if (keyType == 0x30) {
        //tdes key
        this.cryptoWayMap = [
          "tdes-cbc",
          "tdes-ctr",
          "tdes-ecb",
          "tdes-cfb",
          "tdes-ofb",
        ];
      } else if (keyType == 0x97) {
        this.cryptoWayMap = ["rsa-no-padding", "rsassa-pcks1-v1_5", "rsa-oaep"];
      }
    },
  },
};
</script>

<style>
</style>