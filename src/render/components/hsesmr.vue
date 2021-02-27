<template>
  <div>
    <div>
      <el-collapse v-model="activeNames">
        <el-collapse-item title="Add SMR" name="1">
          <el-form
            label-position="left"
            label-width="80px"
            :model="addSmr"
            size="mini"
            ref="smr"
          >
            <el-form-item label="SmrEntry">
              <el-input-number v-model="addSmr.index" controls-position="right" :min="0" :max="7"></el-input-number>
            </el-form-item>
            <el-form-item label="StartAddr">
              <el-input v-model="addSmr.startAddr"></el-input>
            </el-form-item>
            <el-form-item label="Length">
              <el-input v-model="addSmr.length"></el-input>
            </el-form-item>
          </el-form>
        </el-collapse-item>
        <el-collapse-item title="SMR Table" name="2"> </el-collapse-item>
      </el-collapse>
    </div>
    <div></div>
  </div>
</template>

<script>
export default {
  data() {
    return {
      activeNames: ['1'],
      addSmr: {
        index: 0,
        startAddr: 0,
        length: 0,
        proof: false,
        proofAddr: 0,
        key: 0,
      },
    };
  },
};
</script>

<style>
</style>