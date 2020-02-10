<template>
    <div>
        <el-button @click="goPEAK">PEAK</el-button>
    </div>
</template>
<script>
export default {
  methods: {
    goPEAK () {
      this.$router.push('/pcan')
    }
  }
}
</script>
