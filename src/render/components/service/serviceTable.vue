<template>
  <div>
    <el-dialog title="User function" :visible.sync="cd" width="80%">
     
      <el-select v-model="codeIndex" placeholder="请选择" size="mini" style="margin-bottom:20px;width:100%">
        <el-option
          v-for="(n,i) in codeNum"
          :key="i"
          :label="n"
          :value="i">
        </el-option>
      </el-select>
   
      <div>function(writeData,readonly){</div>
      <codemirror v-model="jsFn[codeIndex]"  :options="{readOnly:true}" ref="cmEditor"/>
      <div>}</div>
    </el-dialog>
    <el-table
      size="small"
      ref="basictable"
      row-key="date"
      border
      :data="doipTable"
      style="width: 100%;text-align: center"
    >
      <el-table-column prop="service" label="服务信息" width="300">
        <template
          slot-scope="scope"
        ><div v-if="scope.row.type=='uds'">{{ scope.row.service.name}} (0X{{ scope.row.service.value.toString(16)}})</div>
        <div v-else><el-tag size="mini">Group:</el-tag> <strong>{{ scope.row.service.name}}</strong></div></template>
      </el-table-column>
      <el-table-column label="Suppress" width="76" align="center">
        <template slot-scope="scope" v-if="scope.row.type=='uds'">
          <i class="el-icon-circle-check" v-if="scope.row.payload[0].suppress" style="color:green"></i>
          <i class="el-icon-circle-close" v-else style="color:red"></i>
        </template>
        
      </el-table-column>
      <el-table-column prop="payload" label="Payload">
        <template slot-scope="scope" >
          <div v-if="scope.row.payload&&scope.row.type=='uds'">
            <div v-for="(item, key) in scope.row.payload" :key="key">
              <el-tag size="mini">{{item.name}}</el-tag>
              : {{item[item.name]}}
            </div>
          </div>
          <div v-else-if="scope.row.subtable&&scope.row.type=='group'" style="height:100px;overflow:auto">
            <div v-for="(item, key) in scope.row.subtable" :key="key">
              <div v-if="item.payload">
                <div v-for="(subitem, subkey) in item.payload" :key="subkey">
                  <el-tag size="mini">{{key+'-'+subitem.name}}</el-tag>
                  : {{subitem[subitem.name]}}
                </div>
              </div>
              
            </div>
          </div>
          <div v-else>NULL</div>
        </template>
      </el-table-column>

      <el-table-column prop="func" label="校验函数" width="100"  align="center">
        <template slot-scope="scope">
          <el-button
            type="text"
            icon="el-icon-document"
            @click="showCode(scope.row)"
          ></el-button>
        </template>
      </el-table-column>
      <el-table-column fixed="right" label="操作" width="50">
        <template slot-scope="scope">
          <el-button
            type="danger"
            size="mini"
            icon="el-icon-delete"
            circle
            @click="deleteService(scope.$index)"
            :disabled="running"
          ></el-button>
        </template>
      </el-table-column>
    </el-table>
  </div>
</template>
<script>
import Sortable from "sortablejs";
export default {
  data: function() {
    return {
      sortable: {},
      cd:false,
      jsFn:[" "],
      codeIndex:0,
      codeNum:1
    };
  },
  mounted() {
    const table = document.querySelector(".el-table__body-wrapper tbody");
    const self = this;
    this.sortable = Sortable.create(table, {
      onEnd({ newIndex, oldIndex }) {
        if (self.mode === "can") {
          self.$store.commit("canTableUpdate", [newIndex, oldIndex]);
        } else if (self.mode === "doip") {
          self.$store.commit("doipTableUpdate", [newIndex, oldIndex]);
        } else {
          return;
        }
      }
    });
  },
  props: ["mode"],
  methods: {
    showCode(item){
      this.jsFn=[]
      this.codeIndex=0
      if(item.type=='uds'){
        this.codeNum=1
        this.jsFn[0]=item.func
      }else{
        for(var i in item.subtable){
          this.jsFn[i]=item.subtable[i].func
        }
        this.codeNum=item.subtable.length
      }
     
      this.cd=true
    },
    deleteService(index) {
      if (this.mode === "doip") {
        this.$store.commit("doipTableDelete", index);
      } else if (this.mode === "can") {
        this.$store.commit("canTableDelete", index);
      } else {
        return;
      }
    }
  },
  computed: {
    doipTable: function() {
      if (this.mode === "doip") {
        return this.$store.state.doipTable;
      } else if (this.mode === "can") {
        return this.$store.state.canTable;
      } else {
        return [];
      }
    },
    running: function() {
      return this.$store.state.running;
    }
  }
};
</script>
<style>
.name {
  font-size: 14px;
  font-weight: bolder;
  margin: 5px;
}
.el-table .doip {
  background: #f56c6c;
}
</style>
