<template>
     <div >
         <el-row>
             <el-col :span="24" style="margin:10px">
            长度(byte): <el-input-number v-model="paramLen" controls-position="right" :disabled="minLen==maxLen" :min="minLen"  :max="maxLen" size="mini" @change="numChange"></el-input-number>
             </el-col>
            <el-col :span="24">
            <span v-for="data in paramLen"
            :key="data" style="margin:1px">
                <el-input v-model="paramData[data-1]" @input="dataChange" maxlength="2" show-word-limit size="small" style="width:65px" :readonly="readonly">

                </el-input>
            </span>
            </el-col>
         </el-row>

    </div>
</template>
<script>
export default {
  mounted () {
    this.paramData = new Array(this.paramLen).fill('00')
  },
  props: {
    minLen: {
      type: Number,
      default: function () {
        return 0
      }
    },
    maxLen: {
      type: Number,
      default: function () {
        return 0
      }
    },
    index: {
      type: Number,
      default: function () {
        return 0
      }
    },
    readonly: {
      type: Boolean,
      default: function () {
        return false
      }
    }
  },
  data: function () {
    return {
      paramLen: 0,
      paramData: []
    }
  },
  watch: {
    minLen: function (val) {
      this.paramLen = val
      this.paramData = new Array(this.paramLen).fill('00')
    }
  },
  methods: {
    dataChange () {
      var ret = {}

      ret.index = this.index
      ret.data = this.paramData

      this.$emit('change', ret)
    },
    numChange (val) {
      if (val > this.paramData.length) {
        this.paramData.push('00')
      } else {
        this.paramData.length = val
      }

      var ret = {}

      ret.index = this.index
      ret.data = this.paramData

      this.$emit('change', ret)
    }
  }
}
</script>
