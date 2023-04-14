<template>
  <div>
    <div class="rounter">
      <router-view />
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted, onBeforeUnmount, provide,onUnmounted} from "vue";



/*Window Resize handle*/
const winH = ref( window.innerHeight);
const winW = ref( window.innerWidth);




function resizeHandler() {

  winH.value = window.innerHeight;
  winW.value = window.innerWidth;
}

provide('winH', winH)
provide('winW', winW)

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


.rounter {
  height: v-bind( winH+ "px");
  width: v-bind(winW+'px');
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
