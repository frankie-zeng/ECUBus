<template>
  <div>
    <div>
      <el-card shadow="hover">
        <div slot="header">
          <span>HSE SMR</span>
          <el-button
            style="float: right; padding: 3px 0"
            type="text"
            @click="activeNames = []"
            >Hide</el-button
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
              <el-form-item label="Owner" required prop="keyOwn">
                <el-select v-model="addGroup.keyOwn" placeholder="key_own">
                  <el-option
                    :value="0"
                    label="HSE_KEY_OWNER_ANY"
                    :disabled="addGroup.catalog == 1"
                  >
                  </el-option>
                  <el-option
                    :value="1"
                    label="HSE_KEY_OWNER_CUST"
                    :disabled="addGroup.catalog == 2"
                  >
                  </el-option>
                  <el-option
                    :value="2"
                    label="HSE_KEY_OWNER_OEM"
                    :disabled="addGroup.catalog == 2"
                  >
                  </el-option>
                </el-select>
              </el-form-item>
              <el-form-item label="Key Type" required prop="keyType">
                <el-select v-model="addGroup.keyType" placeholder="key_type">
                  <el-option
                    v-for="(item, key) in keyTypeMap"
                    :key="key"
                    :label="item"
                    :value="key"
                    :disabled="
                      (key == 0x40 && addGroup.catalog == 1) ||
                      (key == 0x97 && addGroup.catalog == 2)
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
                >
                  <el-option
                    v-for="item in keySizeMap"
                    :key="item"
                    :label="item + ' bits'"
                    :value="item"
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
                  <span>{{ keyTypeMap[scope.row.keyType] }}</span>
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
          <el-collapse-item title="RAM Catalog" name="3">
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
                  <span>{{ keyTypeMap[scope.row.keyType] }}</span>
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
import Sortable from "sortablejs";
export default {
  data() {
    return {
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
    sortableEnd(val) {
      let node = this.$refs.nvm.$el.childNodes[2].childNodes[0].childNodes[1]; //tbody
      if (node.isEqualNode(val.from)) {
        let targetRow = this.nvm.splice(val.oldIndex, 1)[0];
        this.nvm.splice(val.newIndex, 0, targetRow);
      } else {
        let targetRow = this.ram.splice(val.oldIndex, 1)[0];
        this.ram.splice(val.newIndex, 0, targetRow);
      }
    },
    catalogChange() {
      this.addGroup.keyOwn = "";
      this.addGroup.keyType = "";
      this.addGroup.keySize = "";
    },
    addGroupHandle() {
      this.$refs.group.validate((valid) => {
        if (valid) {
          this.addGroup.date = new Date().getTime();
          if (this.addGroup.catalog == 1) {
            this.nvm.push(JSON.parse(JSON.stringify(this.addGroup)));
          } else {
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