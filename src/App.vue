<template>
  <div class="main">
    <el-container>
      <el-header :height="headerH + 'px'" />
      <el-container>
        <!-- <el-aside width="200px">Aside</el-aside> -->
        <el-container>
          <el-main style="margin:0px;padding: 0px!important;overflow: hidden !important;;">
            <div class="rounter">
              <router-view />
            </div>
          </el-main>
        </el-container>
      </el-container>
      <el-footer :height="logH + 'px'" />
    </el-container>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted, onBeforeUnmount, provide,onUnmounted} from "vue";



/*Window Resize handle*/
const winH = ref( window.innerHeight);
const collapseWidth=ref(64)
const mainW = ref(window.innerWidth-collapseWidth.value)
const logH = ref(200);
const headerH = ref(42);
const mainH = ref(window.innerHeight - logH.value - headerH.value);


function collapseChanage(val:boolean){
  if(!val){
    collapseWidth.value=150
  }else{
    collapseWidth.value=64
  }
  mainW.value = window.innerWidth-collapseWidth.value;
}

function resizeHandler() {
  mainW.value = window.innerWidth-collapseWidth.value;
  winH.value = window.innerHeight;
  mainH.value = winH.value - logH.value - headerH.value;
}
function logResizeHandler(val: number) {
  logH.value = val;
  resizeHandler();
}
provide('mainW', mainW)
provide('mainH', mainH)
provide('logH', logH)
onMounted(() => {
  resizeHandler();
  window.addEventListener("resize", resizeHandler);
});
onBeforeUnmount(() => {
  window.removeEventListener("resize", resizeHandler);
});
</script>
<style>
body {
  margin: 0px;
  padding: 0px;
  font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Oxygen,
    Ubuntu, Cantarell, "Open Sans", "Helvetica Neue", sans-serif;
  -webkit-user-select: none;
  -webkit-touch-callout: none;
  -moz-user-select: none;
  -ms-user-select: none;
  user-select: none;
  overflow: hidden;
}

.el-dialog__body {
  padding: 10px !important;
}

.el-form-item__content {
  display: block !important;
  text-align: left;
}


.main {
  height: v-bind(winH + "px");
}

.rounter {
  height: v-bind(mainH + "px");
  width: v-bind(mainW+'px');
}

element.style {
  --el-footer-height: 200px;
}

.el-header {
  --el-header-padding: 0px 0px !important
}

.el-footer {
  --el-footer-padding: 0px 2px !important;
}

::-webkit-scrollbar {
  width: 7px;
  height: 5px;
  border-radius: 15px;
  -webkit-border-radius: 15px;
}

::-webkit-scrollbar-track-piece {
  background-color: #ffff;
  border-radius: 15px;
  -webkit-border-radius: 15px;
}

::-webkit-scrollbar-thumb:vertical {
  height: 5px;
  background-color: rgba(144, 147, 153, 0.5);
  border-radius: 15px;
  -webkit-border-radius: 15px;
}

::-webkit-scrollbar-thumb:horizontal {
  width: 7px;
  background-color: rgba(144, 147, 153, 0.5);
  border-radius: 15px;
  -webkit-border-radius: 15px;
}
</style>
