<template>
    <div>
        <el-page-header @back="goBack" content="SECURITY"  class="header" ></el-page-header>
    </div>
</template>
<script>
export default {
    methods:{
        goBack(){
            this.$router.push('/')
        }
    }
}
</script>
<style>
    .header{
        margin: 30px;
    }
</style>