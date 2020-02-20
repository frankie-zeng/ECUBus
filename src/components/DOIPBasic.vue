<template>
    <div>
        <el-table stripe size="small" ref="basictable" row-key="date" border :data="doipTable" style="width: 100%;text-align: center">
            <el-table-column prop="addr" label="地址" width="100">
               <template slot-scope="scope">
                  {{ scope.row.addr.name}}
                </template>
            </el-table-column>
            <el-table-column prop="service" label="服务信息" width="300">
               <template slot-scope="scope">
                  {{ scope.row.service.name}} (0X{{ scope.row.service.value.toString(16)}})
                </template>
            </el-table-column>
            <el-table-column prop="param" label="参数" width="300">
              <template slot-scope="scope">
                  <div v-if="scope.row.param">
                    <div v-for="(item, key) in scope.row.param" :key="key">
                      <el-tag size="mini">{{ item.name}}</el-tag> : {{ item.value}}
                    </div>
                  </div>
                  <div v-else>
                      NULL
                  </div>
                </template>
            </el-table-column>

            <el-table-column prop="func" label="校验函数"  width="500" >
               <template slot-scope="scope">
                  <el-input
                      readonly
                      type="textarea"
                      :value="'function(writeData,readData){\r\n' + scope.row.func +'\r\n}'"
                      :autosize="{ minRows:0 , maxRows: 8}"
                    >
                  </el-input>
                </template>
            </el-table-column>
            <el-table-column
              fixed="right"
              label="操作"
              width="50">
              <template slot-scope="scope">
                <el-button type="danger" size="mini" icon="el-icon-delete" circle @click="deleteService(scope.$index)" :disabled="running"></el-button>
              </template>
            </el-table-column>
        </el-table>
    </div>
</template>
<script>
import Sortable from 'sortablejs'
export default {
  data: function () {
    return {
      sortable: {}
    }
  },
  mounted () {
    const table = document.querySelectorAll('.el-table__body-wrapper tbody')
    const self = this
    this.sortable = Sortable.create(table[this.index], {
      onEnd ({ newIndex, oldIndex }) {
        self.$store.commit('doipTableUpdate', [newIndex, oldIndex])
      }
    })
  },
  props:['index'],
  methods: {
    deleteService (index) {
      this.$store.commit('doipTableDelete', index)
    }
  },
  computed: {
    doipTable: function () {
      return this.$store.state.doipTable
    },
    running: function () {
      return this.$store.state.running
    }
  }
}
</script>
<style>
  .name{
    font-size: 14px;
    font-weight: bolder;
    margin: 5px;

  }
</style>
