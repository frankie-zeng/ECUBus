<template>
  <div>
    <el-dialog :visible.sync="showKeyLoad" width="70%" v-if="showKeyLoad">
      <HseKeyLoad :group="nGroup" />
    </el-dialog>
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
            @click="activeNames = ['1', '2', '3']"
            >Show</el-button
          >
        </div>

        <el-collapse v-model="activeNames">
          <el-collapse-item title="Add Group" name="1">
            <el-form
              label-position="right"
              label-width="180px"
              :model="addGroup"
              size="mini"
              ref="group"
            >
              <el-form-item label="Catalog" required prop="catalog">
                <el-select
                  v-model="addGroup.catalog"
                  placeholder="key_catalog"
                  @change="catalogChange"
                  style="width: 100%"
                >
                  <el-option :value="1" label="HSE_CATALOG_NVM"> </el-option>
                  <el-option :value="2" label="HSE_CATALOG_RAM"> </el-option>
                </el-select>
              </el-form-item>
              <el-form-item label="MU Mask" required prop="muMask">
                <el-checkbox-group v-model="addGroup.muMask" :min="1">
                  <el-checkbox
                    v-for="mu in muMap"
                    :label="mu.value"
                    :key="mu.value"
                    >{{ mu.label }}</el-checkbox
                  >
                </el-checkbox-group>
              </el-form-item>
              <el-form-item label="Key Type" required prop="keyType">
                <el-select
                  v-model="addGroup.keyType"
                  placeholder="key_type"
                  @change="keyTypeChangeHandle"
                  style="width: 100%"
                >
                  <el-option
                    v-for="(item, key) in keyTypeMap"
                    :key="key"
                    :label="item.label"
                    :value="key"
                    :disabled="
                      (key == 0x40 && addGroup.catalog == 1) ||
                      (key == 0x97 && addGroup.catalog == 2)
                    "
                  >
                  </el-option>
                </el-select>
              </el-form-item>
              <el-form-item label="Owner" required prop="keyOwn">
                <el-select
                  v-model="addGroup.keyOwn"
                  placeholder="key_own"
                  style="width: 100%"
                >
                  <el-option :value="0" label="HSE_KEY_OWNER_ANY"> </el-option>
                  <el-option
                    :value="1"
                    label="HSE_KEY_OWNER_CUST"
                    :disabled="
                      addGroup.catalog == 2 || addGroup.keyType == 0x11
                    "
                  >
                  </el-option>
                  <el-option
                    :value="2"
                    label="HSE_KEY_OWNER_OEM"
                    :disabled="
                      addGroup.catalog == 2 || addGroup.keyType == 0x11
                    "
                  >
                  </el-option>
                </el-select>
              </el-form-item>
              <el-form-item label="Key Number" required prop="keyNum">
                <el-input-number
                  v-model="addGroup.keyNum"
                  controls-position="right"
                  :min="1"
                ></el-input-number>
              </el-form-item>
              <el-form-item label="Key Size" required prop="keySize">
                <el-select
                  v-model.number="addGroup.keySize"
                  placeholder="key_size"
                  style="width: 100%"
                >
                  <el-option
                    v-for="item in keySizeMap"
                    :key="item"
                    :label="item + ' bits'"
                    :value="item"
                    :disabled="
                      addGroup.keyType != '' &&
                      keyTypeMap[addGroup.keyType].size.indexOf(item) == -1
                    "
                  >
                  </el-option>
                </el-select>
              </el-form-item>
              <el-form-item>
                <el-button type="primary" @click="addGroupHandle"
                  >Add</el-button
                >
              </el-form-item>
            </el-form>
          </el-collapse-item>
          <el-collapse-item title="NVM Catalog" name="2">
            <el-alert
              title="EXT type key can't load in this tool,you should load key in your firmware!"
              type="info"
              :closable="false"
              style="padding: 5px"
            >
            </el-alert>
            <el-table
              :data="nvm"
              style="width: 100%"
              size="mini"
              ref="nvm"
              row-key="date"
            >
              <el-table-column width="180" label="Group Num">
                <template slot-scope="scope">
                  {{ scope.$index }}
                </template>
              </el-table-column>
              <el-table-column prop="muMask" label="MU Mask" width="180">
                <template slot-scope="scope">
                  <span v-for="mu in scope.row.muMask" :key="mu">{{
                    "MU" + mu + " "
                  }}</span>
                </template>
              </el-table-column>
              <el-table-column prop="keyOwn" label="Key Owner" width="180">
                <template slot-scope="scope">
                  <span v-if="scope.row.keyOwn == 1">HSE_KEY_OWNER_CUST</span>
                  <span v-else-if="scope.row.keyOwn == 2"
                    >HSE_KEY_OWNER_OEM</span
                  >
                  <span v-else>HSE_KEY_OWNER_ANY</span>
                </template>
              </el-table-column>
              <el-table-column prop="keyType" label="Key Type" width="250">
                <template slot-scope="scope">
                  <span>{{ keyTypeMap[scope.row.keyType].label }}</span>
                </template>
              </el-table-column>
              <el-table-column prop="keyNum" label="Key Size">
                <template slot-scope="scope">
                  <span>{{ scope.row.keySize + "bits" }}</span>
                </template>
              </el-table-column>
              <el-table-column prop="keyNum" label="Key Number">
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
                    type="primary"
                    size="mini"
                    icon="el-icon-key"
                    circle
                    @click="viewGroup(scope.row.catalog, scope.$index)"
                    :disabled="
                      scope.row.keyType == 0x89 || scope.row.keyType == 0x99
                    "
                  ></el-button>
                  <el-button
                    type="danger"
                    size="mini"
                    icon="el-icon-delete"
                    circle
                    @click="deleteGroup(scope.row.catalog, scope.$index)"
                  ></el-button>
                </template>
              </el-table-column>
            </el-table>
          </el-collapse-item>
          <el-collapse-item title="RAM Catalog" name="3">
            <el-alert
              title="RAM catalog key can't load in this tool,you should load key in your firmware!"
              type="info"
              :closable="false"
              style="padding: 5px"
            ></el-alert>
            <el-table
              :data="ram"
              style="width: 100%"
              size="mini"
              ref="ram"
              row-key="date"
            >
              <el-table-column width="180" label="Group Num">
                <template slot-scope="scope">
                  {{ scope.$index }}
                </template>
              </el-table-column>
              <el-table-column prop="muMask" label="MU Mask" width="180">
                <template slot-scope="scope">
                  <span v-for="mu in scope.row.muMask" :key="mu">{{
                    "MU" + mu + " "
                  }}</span>
                </template>
              </el-table-column>
              <el-table-column prop="keyOwn" label="Key Owner" width="180">
                <template slot-scope="scope">
                  <span v-if="scope.row.keyOwn == 1">HSE_KEY_OWNER_CUST</span>
                  <span v-else-if="scope.row.keyOwn == 2"
                    >HSE_KEY_OWNER_OEM</span
                  >
                  <span v-else>HSE_KEY_OWNER_ANY</span>
                </template>
              </el-table-column>
              <el-table-column prop="keyType" label="Key Type" width="250">
                <template slot-scope="scope">
                  <span>{{ keyTypeMap[scope.row.keyType].label }}</span>
                </template>
              </el-table-column>
              <el-table-column prop="keyNum" label="Key Size">
                <template slot-scope="scope">
                  <span>{{ scope.row.keySize + "bits" }}</span>
                </template>
              </el-table-column>
              <el-table-column prop="keyNum" label="Key Number">
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
                    @click="deleteGroup(scope.row.catalog, scope.$index)"
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
const keyTypeMap = {
  0x11: {
    label: "HSE_KEY_TYPE_SHE",
    size: [128],
  },
  0x12: {
    label: "HSE_KEY_TYPE_AES",
    size: [128, 192, 256],
  },
  0x20: {
    label: "HSE_KEY_TYPE_HMAC",
    size: [128, 160, 192, 224, 240, 256, 320, 384, 512, 521, 638, 1024],
  },
  0x30: {
    label: "HSE_KEY_TYPE_TDES",
    size: [192],
  },
  0x40: {
    label: "HSE_KEY_TYPE_SHARED_SECRET",
    size: [128, 160, 192, 224, 240, 256, 320, 384, 512, 521, 638, 1024, 2048],
  },
  0x87: {
    label: "HSE_KEY_TYPE_ECC_PAIR",
    size: [192, 224, 240, 256, 320, 384, 512, 521, 638],
  },
  0x88: {
    label: "HSE_KEY_TYPE_ECC_PUB",
    size: [192, 224, 240, 256, 320, 384, 512, 521, 638],
  },
  0x89: {
    label: "HSE_KEY_TYPE_ECC_PUB_EXT",
    size: [192, 224, 240, 256, 320, 384, 512, 521, 638],
  },
  0x97: {
    label: "HSE_KEY_TYPE_RSA_PAIR",
    size: [1024, 2048, 3072, 4096],
  },
  0x98: {
    label: "HSE_KEY_TYPE_RSA_PUB",
    size: [1024, 2048, 3072, 4096],
  },
  0x99: {
    label: "HSE_KEY_TYPE_RSA_PUB_EXT",
    size: [1024, 2048, 3072, 4096],
  },
  0xa7: {
    label: "HSE_KEY_TYPE_DH_PAIR",
    size: [1024, 2048, 3072, 4096],
  },
  0xa8: {
    label: "HSE_KEY_TYPE_DH_PUB",
    size: [1024, 2048, 3072, 3096],
  },
};
import HseKeyLoad from "./hseKeyLoad.vue";
export default {
  components: {
    HseKeyLoad,
  },
  data() {
    return {
      showKeyLoad: false,
      nGroup: {},
      sortable: [],
      activeNames: ["1", "2", "3"],
      nvm: [],
      ram: [],
      keyTypeMap: keyTypeMap,
      muMap: [
        {
          label: "MU0",
          value: 0,
        },
        {
          label: "MU1",
          value: 1,
        },
      ],
      keySizeMap: [
        128,
        160,
        192,
        224,
        240,
        256,
        320,
        384,
        512,
        521,
        638,
        1024,
        2048,
        3072,
        4096,
      ],
      addGroup: {
        catalog: 1,
        date: 0,
        muMask: [0, 1],
        keyOwn: "",
        keyType: "",
        keyNum: 1,
        keySize: "",
      },
    };
  },
  mounted() {
    this.nvm = this.$store.state.hseConfig.format.nvm;
    this.ram = this.$store.state.hseConfig.format.ram;
  },
  watch: {
    watch: {
      nvm: function (val) {
        this.$store.commit("hseFormatLoad", {
          catalog: 1,
          data: val,
        });
      },
      ram: function (val) {
        this.$store.commit("hseFormatLoad", {
          catalog: 2,
          data: val,
        });
      },
    },
  },
  methods: {
    viewGroup(catalog, index) {
      if (catalog == 1) {
        this.nGroup = this.nvm[index];
      } else {
        this.nGroup = this.ram[index];
      }
      this.showKeyLoad = false;
      this.$nextTick(() => {
        this.showKeyLoad = true;
      });
    },
    // sortableEnd(val) {
    //   let node = this.$refs.nvm.$el.childNodes[2].childNodes[0].childNodes[1]; //tbody
    //   if (node.isEqualNode(val.from)) {
    //     let targetRow = this.nvm.splice(val.oldIndex, 1)[0];
    //     this.nvm.splice(val.newIndex, 0, targetRow);
    //   } else {
    //     let targetRow = this.ram.splice(val.oldIndex, 1)[0];
    //     this.ram.splice(val.newIndex, 0, targetRow);
    //   }
    // },
    catalogChange() {
      this.addGroup.keyOwn = "";
      this.addGroup.keyType = "";
      this.addGroup.keySize = "";
      this.$nextTick(() => {
        this.$refs.group.clearValidate();
      });
    },
    keyTypeChangeHandle() {
      this.addGroup.keyOwn = "";
      this.addGroup.keySize = "";
      this.$nextTick(() => {
        this.$refs.group.clearValidate();
      });
    },
    sheKeyCheck() {
      if (this.addGroup.keyType == 0x11) {
        if (this.addGroup.catalog == 1) {
          /*nvm group*/
          if (this.nvm.length > 0) {
            for (let i = 0; i < this.nvm.length; i++) {
              if (this.nvm[i].keyType != 0x11) {
                this.$notify.error({
                  title: "Error",
                  message: "NVM above group must be SHE type",
                  duration: 0,
                });
                return false;
              }
            }
          }
          if (this.nvm.length == 0 && this.addGroup.keyNum > 12) {
            this.$notify.error({
              title: "Error",
              message: "NVM group 0 max length is 12 for SHE",
              duration: 0,
            });
            return false;
          }
          if (this.nvm.length != 0 && this.addGroup.keyNum > 10) {
            this.$notify.error({
              title: "Error",
              message: "NVM group 1~4 max length is 10 for SHE",
              duration: 0,
            });
            return false;
          }

          if (this.nvm.length >= 5) {
            /* she group max length is 5*/
            this.$notify.error({
              title: "Error",
              message: "NVM SHE max group index is 4",
              duration: 0,
            });
            return false;
          }
        } else {
          if (this.ram.length >= 1) {
            /* she group max length is 1*/
            this.$notify.error({
              title: "Error",
              message: "RAM SHE max group index is 0",
              duration: 0,
            });
            return false;
          }
          if (this.addGroup.keyNum > 1) {
            this.$notify.error({
              title: "Error",
              message: "RAM SHE max slot number is 1",
              duration: 0,
            });
            return false;
          }
        }
      }
      return true;
    },
    addGroupHandle() {
      this.$refs.group.validate((valid) => {
        if (valid) {
          /*design rule check*/
          /*she group check*/
          if (!this.sheKeyCheck()) return;
          this.addGroup.date = new Date().getTime();
          this.addGroup.keyHandle = [];

          if (this.addGroup.catalog == 1) {
            for (let i = 0; i < this.addGroup.keyNum; i++) {
              this.addGroup.keyHandle.push({
                handle: (1 << 16) | (this.nvm.length << 8) | i,
                label: `NVM_GROUP${this.nvm.length}_SLOT${i}_${
                  this.keyTypeMap[this.addGroup.keyType].label
                }_${this.addGroup.keySize}BITS`,
                value: {},
                keyLoaded: false,
              });
            }
            this.nvm.push(JSON.parse(JSON.stringify(this.addGroup)));
          } else {
            for (let i = 0; i < this.addGroup.keyNum; i++) {
              this.addGroup.keyHandle.push({
                handle: (2 << 16) | (this.ram.length << 8) | i,
                label: `RAM_GROUP${this.ram.length}_SLOT${i}_${
                  this.keyTypeMap[this.addGroup.keyType].label
                }_${this.addGroup.keySize}BITS`,
                value: {},
                keyLoaded: false,
              });
            }
            this.ram.push(JSON.parse(JSON.stringify(this.addGroup)));
          }
        } else {
          return false;
        }
      });
    },
    deleteGroup(catalog, index) {
      if (catalog == 1) {
        this.nvm.splice(index, 1);
      } else {
        this.ram.splice(index, 1);
      }
    },
  },
};
</script>

<style scoped>
</style>