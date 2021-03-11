<template>
  <div>
    <el-page-header @back="goBack" content="SHE GENERATE" class="header" title=""></el-page-header>
    <div>
      <el-collapse v-model="activeName" accordion style="margin:50px">
        <el-collapse-item title="S32K1 CSEC SHE M1|M2|M3 Generate" name="1">
          <el-form
            :model="k1she"
            :rules="k1rules"
            ref="k1she"
            label-width="120px"
            label-position="right"
            size="small"
          >
            <el-form-item label="Key Name:" prop="name">
              <el-input v-model="k1she.name" style="width:300px"></el-input>
            </el-form-item>
            <el-form-item label="Auth Key ID:" prop="authKeyId">
              <el-input
                v-model.number="k1she.authKeyId"
                style="width:100px"
                maxlength="2"
                show-word-limit
              ></el-input>
            </el-form-item>
            <el-form-item label="Auth Key Vaue:" prop="authKeyValue">
              <el-input
                v-model="k1she.authKeyValue"
                placeholder="hex"
                maxlength="32"
                show-word-limit
              ></el-input>
            </el-form-item>
            <el-form-item label="New Key ID:" prop="keyId">
              <el-input
                v-model.number="k1she.keyId"
                style="width:100px"
                maxlength="2"
                show-word-limit
              ></el-input>
            </el-form-item>
            <el-form-item label="New Key Vaue:" prop="keyValue">
              <el-input v-model="k1she.keyValue" placeholder="hex" maxlength="32" show-word-limit></el-input>
            </el-form-item>
            <el-form-item label="Flags:" prop="flag">
              <el-checkbox-group v-model="k1she.flag">
                <el-checkbox label="WRITE_PROT" name="type"></el-checkbox>
                <el-checkbox label="BOOT_PROT" name="type"></el-checkbox>
                <el-checkbox label="DEBUG_PROT" name="type"></el-checkbox>
                <el-checkbox label="KEY_USAGE" name="type"></el-checkbox>
                <el-checkbox label="WILD_CARD" name="type"></el-checkbox>
                <el-checkbox label="VERIFY_ONLY" name="type"></el-checkbox>
              </el-checkbox-group>
            </el-form-item>
            <el-form-item label="UID:" prop="uid">
              <el-input v-model="k1she.uid" placeholder="hex" maxlength="30" show-word-limit></el-input>
            </el-form-item>
            <el-form-item label="Counter:" prop="counter">
              <el-input-number v-model="k1she.counter" controls-position="right" :min="1"></el-input-number>
            </el-form-item>
            <el-form-item style="text-align:right">
              <el-button type="primary" @click="k1Cal('k1she')">Calculate</el-button>
            </el-form-item>
          </el-form>
        </el-collapse-item>
        <el-collapse-item title="S32K2TV HSE M1|M2|M3 Generate(Legacy)" name="2">
          <el-form
            :model="k2she"
            :rules="k3rules"
            ref="k2she"
            label-width="120px"
            label-position="right"
            size="small"
          >
            <el-form-item label="Key Name:" prop="name">
              <el-input v-model="k2she.name" style="width:300px"></el-input>
            </el-form-item>
            <el-form-item label="Auth Key ID:" prop="authKeyId">
               <el-select v-model="k2she.authKeyId" placeholder="请选择" style="width:300px" filterable>
                <el-option
                  v-for="item in keyArray"
                  :key="item.label"
                  :label="item.label"
                  :value="item.value">
                </el-option>
              </el-select>
            </el-form-item>
            <el-form-item label="Auth Key Vaue:" prop="authKeyValue">
              <el-input
                v-model="k2she.authKeyValue"
                placeholder="hex"
                maxlength="32"
                show-word-limit
              ></el-input>
            </el-form-item>
            <el-form-item label="New Key ID:" prop="keyId">
              <el-select v-model="k2she.keyId" placeholder="请选择" style="width:300px" filterable>
                <el-option
                  v-for="item in keyArray"
                  :key="item.label"
                  :label="item.label"
                  :value="item.value">
                </el-option>
              </el-select>
            </el-form-item>
            <el-form-item label="New Key Vaue:" prop="keyValue">
              <el-input v-model="k2she.keyValue" placeholder="hex" maxlength="32" show-word-limit></el-input>
            </el-form-item>
            <el-form-item label="Flags:" prop="flag">
              <el-checkbox-group v-model="k2she.flag">
                <el-checkbox label="VERIFY_ONLY" name="type"></el-checkbox>
                <el-checkbox label="WRITE_PROT" name="type"></el-checkbox>
                <el-checkbox label="BOOT_PROT" name="type"></el-checkbox>
                <el-checkbox label="DEBUG_PROT" name="type"></el-checkbox>
                <el-checkbox label="KEY_USAGE" name="type"></el-checkbox>
                <el-checkbox label="WILD_CARD" name="type"></el-checkbox>
                <el-checkbox label="EXPORT_RAM" name="type"></el-checkbox>
              </el-checkbox-group>
            </el-form-item>
            <el-form-item label="UID:" prop="uid">
              <el-input v-model="k2she.uid" placeholder="hex" maxlength="16" show-word-limit></el-input>
            </el-form-item>
            <el-form-item label="Counter:" prop="counter">
              <el-input-number v-model="k2she.counter" controls-position="right" :min="1"></el-input-number>
            </el-form-item>
            <el-form-item style="text-align:right">
              <el-button type="primary" @click="k2Cal('k2she')">Calculate</el-button>
            </el-form-item>
          </el-form>
        </el-collapse-item>
        <el-collapse-item title="S32K3 HSE M1|M2|M3 Generate" name="3">
          <el-form
            :model="k3she"
            :rules="k3rules"
            ref="k3she"
            label-width="120px"
            label-position="right"
            size="small"
          >
            <el-form-item label="Key Name:" prop="name">
              <el-input v-model="k3she.name" style="width:300px"></el-input>
            </el-form-item>
            <el-form-item label="Auth Key ID:" prop="authKeyId">
               <el-select v-model="k3she.authKeyId" placeholder="请选择" style="width:300px" filterable>
                <el-option
                  v-for="item in keyArray"
                  :key="item.label"
                  :label="item.label"
                  :value="item.value">
                </el-option>
              </el-select>
            </el-form-item>
            <el-form-item label="Auth Key Vaue:" prop="authKeyValue">
              <el-input
                v-model="k3she.authKeyValue"
                placeholder="hex"
                maxlength="32"
                show-word-limit
              ></el-input>
            </el-form-item>
            <el-form-item label="New Key ID:" prop="keyId">
              <el-select v-model="k3she.keyId" placeholder="请选择" style="width:300px" filterable>
                <el-option
                  v-for="item in keyArray"
                  :key="item.label"
                  :label="item.label"
                  :value="item.value">
                </el-option>
              </el-select>
            </el-form-item>
            <el-form-item label="New Key Vaue:" prop="keyValue">
              <el-input v-model="k3she.keyValue" placeholder="hex" maxlength="32" show-word-limit></el-input>
            </el-form-item>
            <el-form-item label="Flags:" prop="flag">
              <el-checkbox-group v-model="k3she.flag">
                <el-checkbox label="WRITE_PROT" name="type"></el-checkbox>
                <el-checkbox label="BOOT_PROT" name="type"></el-checkbox>
                <el-checkbox label="DEBUG_PROT" name="type"></el-checkbox>
                <el-checkbox label="KEY_USAGE" name="type"></el-checkbox>
                <el-checkbox label="WILD_CARD" name="type"></el-checkbox>
                <el-checkbox label="VERIFY_ONLY" name="type"></el-checkbox>
              </el-checkbox-group>
            </el-form-item>
            <el-form-item label="UID:" prop="uid">
              <el-input v-model="k3she.uid" placeholder="hex" maxlength="16" show-word-limit></el-input>
            </el-form-item>
            <el-form-item label="Counter:" prop="counter">
              <el-input-number v-model="k3she.counter" controls-position="right" :min="1"></el-input-number>
            </el-form-item>
            <el-form-item style="text-align:right">
              <el-button type="primary" @click="k3Cal('k3she')">Calculate</el-button>
            </el-form-item>
          </el-form>
        </el-collapse-item>
        <div>
          <el-button
            type="text"
            icon="el-icon-document-copy"
            class="copy-btn"
            :data-clipboard-text="mma"
          ></el-button>
          <el-input type="textarea" autosize placeholder v-model="mma"></el-input>
        </div>
      </el-collapse>
    </div>
  </div>
</template>
<script>
const { ipcRenderer } = require("electron");
const ClipboardJS = require("clipboard/dist/clipboard.js");

const sprintf = require("sprintf-js").sprintf;
export default {
  mounted() {
    this.clipboard = new ClipboardJS(".copy-btn");
    this.clipboard.on("success", () => {
      this.$notify({
        title: "成功",
        message: "复制成功",
        type: "success"
      });
    });
  },
  data() {
    return {
      clipboard: "",
      activeName: "",
      keyArray:[
        {
          label:'MASTER_ECU_KEY',
          value:[0,1]
        },
        {
          label:'BOOT_MAC_KEY',
          value:[0,2]
        },
        {
          label:'BOOT_MAC',
          value:[0,3]
        },
        {
          label:'RAM_KEY',
          value:[0,14]
        },
        {
          label:'KEY1',
          value:[0,4]
        },
        {
          label:'KEY2',
          value:[0,5]
        },
        {
          label:'KEY3',
          value:[0,6]
        },
        {
          label:'KEY4',
          value:[0,7]
        },
        {
          label:'KEY5',
          value:[0,8]
        },
        {
          label:'KEY6',
          value:[0,9]
        },
        {
          label:'KEY7',
          value:[0,10]
        },
        {
          label:'KEY8',
          value:[0,11]
        },
        {
          label:'KEY9',
          value:[0,12]
        },
        {
          label:'KEY10',
          value:[0,13]
        },
        {
          label:'KEY11',
          value:[1,4]
        },
        {
          label:'KEY12',
          value:[1,5]
        },
        {
          label:'KEY13',
          value:[1,6]
        },
        {
          label:'KEY14',
          value:[1,7]
        },
        {
          label:'KEY15',
          value:[1,8]
        },
        {
          label:'KEY16',
          value:[1,9]
        },
        {
          label:'KEY17',
          value:[1,10]
        },
        {
          label:'KEY18',
          value:[1,11]
        },
        {
          label:'KEY19',
          value:[1,12]
        },
        {
          label:'KEY20',
          value:[1,13]
        },
        {
          label:'KEY21',
          value:[2,4]
        },
        {
          label:'KEY22',
          value:[2,5]
        },
        {
          label:'KEY23',
          value:[2,6]
        },
        {
          label:'KEY24',
          value:[2,7]
        },
        {
          label:'KEY25',
          value:[2,8]
        },
        {
          label:'KEY26',
          value:[2,9]
        },
        {
          label:'KEY27',
          value:[2,10]
        },
        {
          label:'KEY28',
          value:[2,11]
        },
        {
          label:'KEY29',
          value:[2,12]
        },
        {
          label:'KEY30',
          value:[2,13]
        },
        {
          label:'KEY31',
          value:[3,4]
        },
        {
          label:'KEY32',
          value:[3,5]
        },
        {
          label:'KEY33',
          value:[3,6]
        },
        {
          label:'KEY34',
          value:[3,7]
        },
        {
          label:'KEY35',
          value:[3,8]
        },
        {
          label:'KEY36',
          value:[3,9]
        },
        {
          label:'KEY37',
          value:[3,10]
        },
        {
          label:'KEY38',
          value:[3,11]
        },
        {
          label:'KEY39',
          value:[3,12]
        },
        {
          label:'KEY40',
          value:[3,13]
        },
        {
          label:'KEY41',
          value:[4,4]
        },
        {
          label:'KEY42',
          value:[4,5]
        },
        {
          label:'KEY43',
          value:[4,6]
        },
        {
          label:'KEY44',
          value:[4,7]
        },
        {
          label:'KEY45',
          value:[4,8]
        },
        {
          label:'KEY46',
          value:[4,9]
        },
        {
          label:'KEY47',
          value:[4,10]
        },
        {
          label:'KEY48',
          value:[4,11]
        },
        {
          label:'KEY49',
          value:[4,12]
        },
        {
          label:'KEY50',
          value:[4,13]
        },
      ],
      k1she: {
        name: "test",
        authKeyId: 2,
        authKeyValue: "492c29292b031f4571a870207c47056e",
        keyId: 1,
        keyValue: "492c29292b031f4571a870207c47056e",
        flag: [],
        // flag:{
        //     WRITE_PROT:false,
        //     BOOT_PROT:false,
        //     DEBUG_PROT:false,
        //     KEY_USAGE:false,
        //     WILD_CARD:false,
        //     VERIFY_ONLY:false
        // },
        counter: 1,
        uid: "492c29292b031f4571a870207c4705"
      },
      k3she: {
        name: "test",
        authKeyId: '',
        authKeyValue: "492c29292b031f4571a870207c47056e",
        keyId: '',
        keyValue: "492c29292b031f4571a870207c47056e",
        flag: [],
        // flag:{
        //     WRITE_PROT:false,
        //     BOOT_PROT:false,
        //     DEBUG_PROT:false,
        //     KEY_USAGE:false,
        //     WILD_CARD:false,
        //     VERIFY_ONLY:false
        // },
        counter: 1,
        uid: "4571a870207c4705"
      },
      k2she: {
        name: "test",
        authKeyId: '',
        authKeyValue: "492c29292b031f4571a870207c47056e",
        keyId: '',
        keyValue: "492c29292b031f4571a870207c47056e",
        flag: [],
        // flag:{
        //     WRITE_PROT:false,
        //     BOOT_PROT:false,
        //     DEBUG_PROT:false,
        //     KEY_USAGE:false,
        //     WILD_CARD:false,
        //     VERIFY_ONLY:false
        // },
        counter: 1,
        uid: "4571a870207c4705"
      },
      mma:
        "/* S32K1/K3 M1M2M3 generated by ECUBus,https://github.com/frankie-zeng/ECUBus*/\r\n",
      k1rules: {
        name: [{ required: true, message: "请输入KEY的名字", trigger: "blur" }],
        authKeyId: [
          {
            required: true,
            message: "请输入用于验证的KEY-ID，必须是数字",
            trigger: "blur",
            type: "number"
          }
        ],
        authKeyValue: [
          {
            required: true,
            pattern: /^[0-9a-fA-F]+$/,
            message: '请输入用于验证的KEY值，eg:"0001020304"',
            trigger: "change"
          },
          { min: 32, max: 32, message: "KEY的长度为16个字节", trigger: "blur" }
        ],
        keyId: [
          {
            required: true,
            message: "请输入用于验证的KEY-ID，必须是数字",
            trigger: "blur",
            type: "number"
          }
        ],
        keyValue: [
          {
            required: true,
            pattern: /^[0-9a-fA-F]+$/,
            message: '请输入用于验证的KEY值，HEX格式的形式，eg:"0001020304"',
            trigger: "change"
          },
          { min: 32, max: 32, message: "KEY的长度为16个字节", trigger: "blur" }
        ],
        uid: [
          {
            required: true,
            pattern: /^[0-9a-fA-F]+$/,
            message: '请输入用于验证的KEY值，eg:"0001020304"',
            trigger: "change"
          },
          { min: 30, max: 30, message: "UID的长度为15个字节", trigger: "blur" }
        ]
      },
      k3rules: {
        name: [{ required: true, message: "请输入KEY的名字", trigger: "blur" }],
        authKeyId: [
          {
            required: true,
            message: "请输入用于验证的KEY-ID",
            trigger: "blur",
          }
        ],
        authKeyValue: [
          {
            required: true,
            pattern: /^[0-9a-fA-F]+$/,
            message: '请输入用于验证的KEY值，eg:"0001020304"',
            trigger: "change"
          },
          { min: 32, max: 32, message: "KEY的长度为16个字节", trigger: "blur" }
        ],
        keyId: [
          {
            required: true,
            message: "请输入用于验证的KEY-ID",
            trigger: "blur",
          }
        ],
        keyValue: [
          {
            required: true,
            pattern: /^[0-9a-fA-F]+$/,
            message: '请输入用于验证的KEY值，HEX格式的形式，eg:"0001020304"',
            trigger: "change"
          },
          { min: 32, max: 32, message: "KEY的长度为16个字节", trigger: "blur" }
        ],
        uid: [
          {
            required: true,
            pattern: /^[0-9a-fA-F]+$/,
            message: '请输入用于验证的KEY值，eg:"0001020304"',
            trigger: "change"
          },
          { min: 16, max: 16, message: "UID的长度为8个字节", trigger: "blur" }
        ]
      }
    };
  },
  methods: {
    goBack() {
      this.$router.push("/");
    },
    k1Cal(formName) {
      this.mma =
        "/* S32K1 CSEC M1M2M3 generated by ECUBus,https://github.com/frankie-zeng/ECUBus*/\r\n";
      this.$refs[formName].validate(valid => {
        if (valid) {
          //console.log(this.k1she)
          var a;
          var i;
          var mstr = "";
          var m = ipcRenderer.sendSync("k1SHEGen", this.k1she);
          this.mma += sprintf("/* %s M1,M2,M3 */\r\n", this.k1she.name);
          /* M1 */
          a = [];
          mstr = Buffer.from(m.M1).toString("hex");
          for (i = 0; i < mstr.length / 2; i++) {
            a.push("0x" + mstr.slice(i * 2, (i + 1) * 2));
          }
          this.mma += sprintf(
            "uint8_t %sM1[16]={%s};\r\n",
            this.k1she.name,
            a.toString("hex")
          );
          /* M2 */
          a = [];
          mstr = Buffer.from(m.M2).toString("hex");
          for (i = 0; i < mstr.length / 2; i++) {
            a.push("0x" + mstr.slice(i * 2, (i + 1) * 2));
          }
          this.mma += sprintf(
            "uint8_t %sM2[32]={%s};\r\n",
            this.k1she.name,
            a.toString("hex")
          );
          /* M3 */
          a = [];
          mstr = Buffer.from(m.M3).toString("hex");
          for (i = 0; i < mstr.length / 2; i++) {
            a.push("0x" + mstr.slice(i * 2, (i + 1) * 2));
          }
          this.mma += sprintf(
            "uint8_t %sM3[16]={%s};\r\n",
            this.k1she.name,
            a.toString("hex")
          );
          /* M4 */
          a = [];
          mstr = Buffer.from(m.M4).toString("hex");
          for (i = 0; i < mstr.length / 2; i++) {
            a.push("0x" + mstr.slice(i * 2, (i + 1) * 2));
          }
          this.mma += sprintf(
            "uint8_t %sM4[32]={%s};\r\n",
            this.k1she.name,
            a.toString("hex")
          );
          /* M5 */
          a = [];
          mstr = Buffer.from(m.M5).toString("hex");
          for (i = 0; i < mstr.length / 2; i++) {
            a.push("0x" + mstr.slice(i * 2, (i + 1) * 2));
          }
          this.mma += sprintf(
            "uint8_t %sM5[16]={%s};\r\n",
            this.k1she.name,
            a.toString("hex")
          );
          this.mma += "/***************/\r\n";
        }
      });
    },
    k2Cal(formName) {
      this.mma =
        "/* S32K2TV HSE M1M2M3 generated by ECUBus,https://github.com/frankie-zeng/ECUBus*/\r\n";
      this.$refs[formName].validate(valid => {
        if (valid) {
          //console.log(this.k1she)
          var a;
          var i;
          var mstr = "";
          var m = ipcRenderer.sendSync("k2SHEGen", this.k2she);
          this.mma += sprintf("/* %s M1,M2,M3 */\r\n", this.k2she.name);
          /* M1 */
          a = [];
          mstr = Buffer.from(m.M1).toString("hex");
          for (i = 0; i < mstr.length / 2; i++) {
            a.push("0x" + mstr.slice(i * 2, (i + 1) * 2));
          }
          this.mma += sprintf(
            "uint8_t %sM1[16]={%s};\r\n",
            this.k2she.name,
            a.toString("hex")
          );
          /* M2 */
          a = [];
          mstr = Buffer.from(m.M2).toString("hex");
          for (i = 0; i < mstr.length / 2; i++) {
            a.push("0x" + mstr.slice(i * 2, (i + 1) * 2));
          }
          this.mma += sprintf(
            "uint8_t %sM2[32]={%s};\r\n",
            this.k2she.name,
            a.toString("hex")
          );
          /* M3 */
          a = [];
          mstr = Buffer.from(m.M3).toString("hex");
          for (i = 0; i < mstr.length / 2; i++) {
            a.push("0x" + mstr.slice(i * 2, (i + 1) * 2));
          }
          this.mma += sprintf(
            "uint8_t %sM3[16]={%s};\r\n",
            this.k2she.name,
            a.toString("hex")
          );
          /* M4 */
          a = [];
          mstr = Buffer.from(m.M4).toString("hex");
          for (i = 0; i < mstr.length / 2; i++) {
            a.push("0x" + mstr.slice(i * 2, (i + 1) * 2));
          }
          this.mma += sprintf(
            "uint8_t %sM4[32]={%s};\r\n",
            this.k2she.name,
            a.toString("hex")
          );
          /* M5 */
          a = [];
          mstr = Buffer.from(m.M5).toString("hex");
          for (i = 0; i < mstr.length / 2; i++) {
            a.push("0x" + mstr.slice(i * 2, (i + 1) * 2));
          }
          this.mma += sprintf(
            "uint8_t %sM5[16]={%s};\r\n",
            this.k2she.name,
            a.toString("hex")
          );
          this.mma += "/***************/\r\n";
        }
      });
    },
    k3Cal(formName) {
      this.mma =
        "/* S32K3 HSE M1M2M3 generated by ECUBus,https://github.com/frankie-zeng/ECUBus*/\r\n";
      this.$refs[formName].validate(valid => {
        if (valid) {
          //console.log(this.k1she)
          var a;
          var i;
          var mstr = "";
          var m = ipcRenderer.sendSync("k3SHEGen", this.k3she);
          this.mma += sprintf("/* %s M1,M2,M3 */\r\n", this.k3she.name);
          /* M1 */
          a = [];
          mstr = Buffer.from(m.M1).toString("hex");
          for (i = 0; i < mstr.length / 2; i++) {
            a.push("0x" + mstr.slice(i * 2, (i + 1) * 2));
          }
          this.mma += sprintf(
            "uint8_t %sM1[16]={%s};\r\n",
            this.k3she.name,
            a.toString("hex")
          );
          /* M2 */
          a = [];
          mstr = Buffer.from(m.M2).toString("hex");
          for (i = 0; i < mstr.length / 2; i++) {
            a.push("0x" + mstr.slice(i * 2, (i + 1) * 2));
          }
          this.mma += sprintf(
            "uint8_t %sM2[32]={%s};\r\n",
            this.k3she.name,
            a.toString("hex")
          );
          /* M3 */
          a = [];
          mstr = Buffer.from(m.M3).toString("hex");
          for (i = 0; i < mstr.length / 2; i++) {
            a.push("0x" + mstr.slice(i * 2, (i + 1) * 2));
          }
          this.mma += sprintf(
            "uint8_t %sM3[16]={%s};\r\n",
            this.k3she.name,
            a.toString("hex")
          );
          /* M4 */
          a = [];
          mstr = Buffer.from(m.M4).toString("hex");
          for (i = 0; i < mstr.length / 2; i++) {
            a.push("0x" + mstr.slice(i * 2, (i + 1) * 2));
          }
          this.mma += sprintf(
            "uint8_t %sM4[32]={%s};\r\n",
            this.k3she.name,
            a.toString("hex")
          );
          /* M5 */
          a = [];
          mstr = Buffer.from(m.M5).toString("hex");
          for (i = 0; i < mstr.length / 2; i++) {
            a.push("0x" + mstr.slice(i * 2, (i + 1) * 2));
          }
          this.mma += sprintf(
            "uint8_t %sM5[16]={%s};\r\n",
            this.k3she.name,
            a.toString("hex")
          );
          this.mma += "/***************/\r\n";
        }
      });
    }
  }
};
</script>
<style>
.header {
  margin: 30px;
}
.copy-btn {
  position: absolute;
  z-index: 2;
  right: 100px;
}
</style>