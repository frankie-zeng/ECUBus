<template>
    <div class="connect">
        <el-form  :model="tpConfig">
            <el-form-item label="Name:" required>
              <el-input  v-model="tpConfig.name" placeholder="Name" maxlength="20" style="width:180px"></el-input>
            </el-form-item>
            <el-form-item label="组播地址:" required>
                <el-input  v-model="tpConfig.multicast" placeholder="Name" maxlength="20" style="width:180px"></el-input>
            </el-form-item>
            <el-form-item required>
                <el-col :span="10">
                    <div>SA:</div><el-input style="width:250px" v-model="tpConfig.SA" placeholder="SA" maxlength="5" show-word-limit ></el-input>
                </el-col>
                <el-col :span="10" :offset="1">
                    <div>TA:</div><el-input  style="width:250px" v-model="tpConfig.TA" placeholder="TA" maxlength="5" show-word-limit></el-input>
                </el-col>
            </el-form-item>
            <el-form-item style="text-align:right">
                <el-button @click="setupAddr" type="primary" plain >ADD</el-button>
            </el-form-item>
        </el-form>
        
        <el-table
            size="mini"
            :data="addrTable"
            style="width: 100%">

            <el-table-column
                type="index"
                width="50"
                align="center">
            </el-table-column>
            <el-table-column
                prop="name"
                label="名字"
                width="150"
                align="center">
            </el-table-column>
            <el-table-column
                prop="multicast"
                label="组播地址"
                width="150"
                align="center">
            </el-table-column>
            <el-table-column
                prop="SA"
                label="SA"
                width="100"
                align="center">
                <template slot-scope="scope">
                    <el-tag size="medium"  type="info">{{scope.row.SA}}</el-tag>
                </template>
            </el-table-column>
            <el-table-column
                prop="TA"
                label="TA"
                width="100"
                align="center">
                <template slot-scope="scope">
                    <el-tag size="medium"  type="info">{{scope.row.TA}}</el-tag>
                </template>
            </el-table-column>
           
            <el-table-column label="操作"
            align="center">
            <template slot-scope="scope">
                <el-button
                size="mini"
                type="danger"
                @click="handleDelete(scope.$index)">删除</el-button>
            </template>
            </el-table-column>
        </el-table>

    </div>
</template>
<script>
//import PCANTP from './../../build/Release/PCANTP.node'

// const { ipcRenderer } = require('electron')
export default {

  data: function () {
    return {
      tpConfig: {
        name: 'defaultName1',
        SA: 0,
        TA: 0,
        multicast:'255.255.255.255',
      },
    }
  },
  computed: {
    addrTable: function () {
      return this.$store.state.doipAddrTable
    },
  },
  methods: {
    handleDelete (index) {
      this.$store.commit('doipAddrDelete', index)
    },
    setupAddr () {
      var item = JSON.parse(JSON.stringify(this.tpConfig))
      for (var i in this.addrTable) {
        if (this.addrTable[i].name === item.name) {
          this.$notify.error({
            title: 'Error',
            message: 'Name重复'
          })
          return
        }
      }
      if (item.SA === item.TA) {
        this.$notify.error({
          title: 'Error',
          message: 'SA不能等于TA'
        })
        return
      }
      item.SA = parseInt(item.SA, 10)
      item.TA = parseInt(item.TA, 10)
      this.$store.commit('doipAddrAdd', item)
    },
  }
}
</script>
<style>
    .el-row{
        margin-bottom: 10px;
    }
    .header{
        font-weight: 800;
    }
    .title{
        line-height: 18px;
        font-weight: 550
    }

</style>
