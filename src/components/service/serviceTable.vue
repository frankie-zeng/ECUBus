<template>
    <div>
        <el-table size="small" ref="basictable" row-key="date" border :data="doipTable" style="width: 100%;text-align: center"
        :row-class-name="tableRowClassName">
            
            <el-table-column prop="service" label="服务信息" width="300">
               <template slot-scope="scope">
                  {{ scope.row.service.name}} (0X{{ scope.row.service.value.toString(16)}})
                </template>
            </el-table-column>
            <el-table-column prop="addr" label="地址" width="100">
               <template slot-scope="scope">
                  {{ scope.row.addr.name}},SA:{{ scope.row.addr.sa}},TA:{{ scope.row.addr.ta}}
                </template>
            </el-table-column>
           <el-table-column prop="payload" label="Payload" width="300">
              <template slot-scope="scope">
                  <div v-if="scope.row.payload">
                    <div v-for="(item, key) in scope.row.payload" :key="key">
                      <el-tag size="mini">{{ key}}</el-tag> : {{ item}}
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
            <el-table-column prop="other" label="其他">
              <template slot-scope="scope">
                  <div v-if="scope.row.other">
                    <div>
                      <el-tag size="mini">PATH</el-tag> : {{scope.row.other.filePath}}
                    </div>
                    <div>
                      <el-tag size="mini">SIZE</el-tag> : {{scope.row.other.fileSize}}
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
    const table = document.querySelectorAll('.el-table__body-wrapper tbody')
    const self = this
    this.sortable = Sortable.create(table[this.index], {
      onEnd ({ newIndex, oldIndex }) {
        self.$store.commit('doipTableUpdate', [newIndex, oldIndex])
      }
    })
  },
  props:['index','mode'],
  methods: {
    deleteService (index) {
        if(this.mode==='doip'){
            this.$store.commit('doipTableDelete', index)
        }else if(this.mode==='can'){
            this.$store.commit('canTableDelete',index)
        }else{
            return 
        }
    },
    // eslint-disable-next-line no-unused-vars
    tableRowClassName({row, rowIndex}) {
        if (row.type === 'uds') {
            return 'uds';
        } else if (row.type==='doip') {
            return 'doip';
        }
        return '';
    },
  },
  computed: {
    
   
    doipTable: function () {
        if(this.mode==='doip'){
            return this.$store.state.doipTable
        }else if(this.mode==='can'){
            return this.$store.state.canTable
        }else{
            return []
        }
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
  .el-table .doip {
    background: #F56C6C;
  }
</style>
