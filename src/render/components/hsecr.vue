<template>
  <div>
    <el-card shadow="hover">
      <div slot="header">
        <span>HSE CR</span>
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
        <el-collapse-item title="Add CR" name="1">
          <el-alert
            title="CR install need user upload firmware firstly!"
            type="info"
            :closable="false"
            style="padding: 5px; margin-bottom: 20px"
            v-if="!isFw"
          ></el-alert>
          <el-form
            label-position="left"
            label-width="150px"
            :model="addCr"
            size="mini"
            ref="cr"
            :rules="rules"
            :disabled="!isFw"
          >
            <el-form-item label="CrEntry" prop="index" required>
              <el-input-number
                v-model="addCr.index"
                controls-position="right"
                :min="0"
                :max="1"
              ></el-input-number>
            </el-form-item>
            <el-form-item label="Sanction" prop="sanction" required>
              <el-select
                v-model="addCr.sanction"
                placeholder="请选择"
                style="width: 100%"
              >
                <el-option label="DIS_INDIV_KEYS" :value="0"></el-option>
                <el-option label="KEEP_CORE_IN_RESET" :value="1"></el-option>
                <el-option label="RESET_SOC" :value="2"></el-option>
                <el-option label="DIS_ALL_KEYS" :value="3"></el-option>
              </el-select>
            </el-form-item>
            <el-form-item label="Core ID" prop="coreId" required>
              <el-select
                v-model="addCr.coreId"
                placeholder="请选择"
                style="width: 100%"
              >
                <el-option label="HSE_APP_CORE0" :value="0"></el-option>
                <el-option label="HSE_APP_CORE1" :value="1"></el-option>
              </el-select>
            </el-form-item>
            <el-form-item label="SMR Verify Map:" prop="smrVerifMap" required>
              <el-checkbox-group v-model="addCr.smrVerifMap">
                <el-checkbox
                  :label="'SMR #:' + item.index"
                  name="type"
                  v-for="item in smrTable"
                  :key="item.index"
                ></el-checkbox>
              </el-checkbox-group>
            </el-form-item>
            <el-form-item label="Pass Reset Addr" prop="passRest" required>
              <el-input v-model="addCr.passRest" :maxlength="8" show-word-limit>
                <template slot="prepend">0x</template></el-input
              >
            </el-form-item>
            <el-form-item label="Alt SMR Verify Map:" prop="altSmrVerifMap">
              <el-checkbox-group v-model="addCr.altSmrVerifMap">
                <el-checkbox
                  :label="'SMR #:' + item.index"
                  name="type"
                  v-for="item in altSmrTable"
                  :key="item.index"
                ></el-checkbox>
              </el-checkbox-group>
            </el-form-item>
            <el-form-item label="Alt Pass Reset Addr" prop="altPassRest">
              <el-input
                v-model="addCr.altPassRest"
                :maxlength="8"
                show-word-limit
              >
                <template slot="prepend">0x</template></el-input
              >
            </el-form-item>
            <el-form-item>
              <el-button type="primary" @click="addCrHandle">Add</el-button>
            </el-form-item>
          </el-form>
        </el-collapse-item>
        <el-collapse-item title="CR Table" name="2">
          <el-table
            :data="crTable"
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
            <el-table-column prop="index" label="CR Entry"> </el-table-column>
            <el-table-column prop="coreId" label="Core ID"> </el-table-column>
            <el-table-column prop="sanction" label="Sanction">
              <template slot-scope="scope">
                <span v-if="scope.row.sanction == 0">DIS_INDIV_KEYS</span>
                <span v-else-if="scope.row.sanction == 1"
                  >KEEP_CORE_IN_RESET</span
                >
                <span v-else-if="scope.row.sanction == 2">RESET_SOC</span>
                <span v-else>DIS_ALL_KEYS</span>
              </template>
            </el-table-column>
            <el-table-column prop="smrVerifMap" label="SMR Verify Map">
            </el-table-column>
            <el-table-column prop="passRest" label="Pass Rest Address">
              <template slot-scope="scope">
                {{ "0x" + scope.row.passRest.toString(16) }}
              </template>
            </el-table-column>
            <el-table-column prop="altSmrVerifMap" label="Alt SMR Verify Map">
            </el-table-column>
            <el-table-column prop="altPassRest" label="Alt Pass Rest Address">
              <template slot-scope="scope">
                {{ "0x" + scope.row.altPassRest.toString(16) }}
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
                  @click="deleteCr(scope.$index)"
                ></el-button>
              </template>
            </el-table-column>
          </el-table>
        </el-collapse-item>
      </el-collapse>
    </el-card>
  </div>
</template>

<script>
export default {
  data() {
    return {
      activeNames: ["1", "2"],
      crTable: [],
      addCr: {
        index: 0,
        coreId: 0,
        sanction: "",
        passRest: "",
        altPassRest: "",
        smrVerifMap: [],
        altSmrVerifMap: [],
      },
      rules: {
        passRest: [
          {
            required: true,
            pattern: /^[0-9a-fA-F]+$/,
            message: 'Please input HEX format，eg:"0001020304"',
            trigger: "change",
          },
        ],
        altPassRest: [
          {
            pattern: /^[0-9a-fA-F]+$/,
            message: 'Please input HEX format，eg:"0001020304"',
            trigger: "change",
          },
        ],
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
    this.crTable = this.$store.state.hseConfig.cr;
  },
  methods: {
    addCrHandle() {
      this.$refs.cr.validate((valid) => {
        if (valid) {
          let clone = JSON.parse(JSON.stringify(this.addCr));
          clone.passRest = parseInt(clone.passRest, 16);
          clone.altPassRest = parseInt(clone.altPassRest, 16);
          for (let i = 0; i < this.crTable.length; i++) {
            if (this.crTable[i].index == this.addCr.index) {
              this.$set(this.crTable, i, clone);
              return;
            }
          }
          this.crTable.push(clone);
        } else {
          return false;
        }
      });
    },
    deleteCr(index) {
      this.crTable.splice(index, 1);
    },
  },
  watch: {
    smrTable: function () {
      this.addCr.smrVerifMap = [];
    },
    altSmrTable: function () {
      this.addCr.altSmrVerifMap = [];
    },
  },
  computed: {
    smrTable: function () {
      let smr = this.$store.state.hseConfig.smr;
      let table = [];
      for (let i = 0; i < smr.length; i++) {
        if (smr[i].verifyWay == 0x56 || smr[i].keyType == 0xa7) {
          table.push(smr[i]);
        }
      }
      return table;
    },
    altSmrTable: function () {
      let smr = this.$store.state.hseConfig.smr;
      let table = [];
      for (let i = 0; i < smr.length; i++) {
        if (smr[i].verifyWay == 0x75) {
          table.push(smr[i]);
        }
      }
      return table;
    },
  },
};
</script>

<style>
</style>