<template>
  <div>
    <el-card shadow="hover">
      <div slot="header">
        <span>HSE Attribute</span>
      </div>
      <el-alert
        title="These attributes are OTP!"
        type="warning"
        show-icon
        :closable="false"
        style="margin-bottom: 10px"
      >
      </el-alert>
      <el-form
        label-position="left"
        label-width="150px"
        :model="attr"
        size="mini"
        :rules="rules"
      >
        <el-form-item label="Password:" prop="password" required>
          <el-input
            v-model="attr.password"
            :maxlength="32"
            show-word-limit
            style="width: 100%"
            ><template slot="prepend">0x</template></el-input
          >
        </el-form-item>
        <el-form-item label="Life Cycle:" required>
          <el-select v-model="attr.lc" style="width: 100%">
            <el-option label="OEM_PROD" value="oem"></el-option>
            <el-option label="IN_FIELD" value="field"></el-option>
          </el-select>
        </el-form-item>
        <el-form-item label="Flag">
          <el-col :span="8">
            <el-form-item>
              <el-checkbox v-model="attr.ivt">IVT_AUTH</el-checkbox>
              <el-tooltip
                effect="dark"
                content="Forces the IVT and CFG authentication check before running the HSE firmware"
                placement="top-start"
              >
                <el-button type="text" icon="el-icon-info"></el-button>
              </el-tooltip>
            </el-form-item>
          </el-col>
          <el-col :span="8">
            <el-form-item>
              <el-checkbox v-model="attr.auth">AUTH_MODE</el-checkbox>
              <el-tooltip effect="dark" placement="top-start">
                <div slot="content">
                  0:static authentication (password)<br />1:dynamic
                  authentication (challenge / response)
                </div>
                <el-button type="text" icon="el-icon-info"></el-button>
              </el-tooltip>
            </el-form-item>
          </el-col>
          <el-col :span="8">
            <el-form-item>
              <el-checkbox v-model="attr.adkp">ADKP</el-checkbox>
              <el-tooltip effect="dark" placement="top-start">
                <div slot="content">
                  the input value is considered as a master debug key and is
                  diversified with the device’s UID
                </div>
                <el-button type="text" icon="el-icon-info"></el-button>
              </el-tooltip>
            </el-form-item>
          </el-col>
        </el-form-item>
      </el-form>
    </el-card>
  </div>
</template>

<script>
export default {
  data() {
    return {
      attr: {
        password: "",
        ivt: false,
        auth: false,
        adkp: false,
        lc: "oem",
      },
      rules: {
        password: [
          {
            required: true,
            pattern: /^[0-9a-fA-F]+$/,
            message: 'Please input HEX format，eg:"0001020304"',
            trigger: "change",
          },
          { min: 32, max: 32, message: "Key needs 16 bytes", trigger: "blur" },
        ],
      },
    };
  },
  mounted() {
    this.attr = this.$store.state.hseConfig.attr;
  },
  watch: {
    attr: function (val) {
      this.$store.commit("hseAttrLoad", val);
    },
  },
};
</script>

<style scoped>
.tip {
  padding-left: 5px;
}
</style>