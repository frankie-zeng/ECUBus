<template>
    <div>
        <el-table stripe size="small" ref="table" row-key="date" border :data="udsTable" style="width: 100%;text-align: center">
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
            <el-table-column prop="subFunction" label="SubFunction" width="180">
              <template slot-scope="scope">
                  <div v-if="scope.row.subFunc">
                     {{ scope.row.subFunc.name}} (0X{{ scope.row.subFunc.value.toString(16)}})
                  </div>
                  <div v-else>
                      NULL
                  </div>
                </template>
            </el-table-column>
            <el-table-column label="Suppress" width="76" align="center">
              <template slot-scope="scope">
                  <div v-if="scope.row.subFunc">
                     <i class="el-icon-circle-check" v-if="scope.row.subFunc.suppress" style="color:green"></i>
                  <i class="el-icon-circle-close" v-else style="color:red"></i>
                  </div>
                  <div v-else>
                      NULL
                  </div>
                </template>
            </el-table-column>
            <el-table-column prop="payload" label="Payload" width="300">
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
            <el-table-column prop="other" label="其他" width="200">
              <template slot-scope="scope">
                  <div v-if="scope.row.other">
                    <div>
                      <el-tag size="mini">PATH</el-tag> : {{scope.row.other.path}}
                    </div>
                    <div>
                      <el-tag size="mini">SIZE</el-tag> : {{scope.row.other.size}}
                    </div>
                  </div>
                  <div v-else>
                      NULL
                  </div>
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
    const table = document.querySelector('.el-table__body-wrapper tbody')
    const self = this
    this.sortable = Sortable.create(table, {
      onEnd ({ newIndex, oldIndex }) {
        self.$store.commit('udsTableUpdate', [newIndex, oldIndex])
      }
    })
  },
  methods: {
    deleteService (index) {
      this.$store.commit('udsTableDelete', index)
    }
  },
  computed: {
    udsTable: function () {
      return this.$store.state.udsTable
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
