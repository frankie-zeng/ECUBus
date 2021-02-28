import Vue from 'vue'
import Vuex from 'vuex'

Vue.use(Vuex)

export default new Vuex.Store({
  state: {
    canConnect: false,
    doipConnect: false,
    linConnect: false,
    logLevel:'info',
    /*addr*/
    canAddrTable: [],
    doipAddrTable: [],
    linAddrTable: [], 
    lpAddrTable:[{
      name:'emulate-lp',
      SA:0,
      TA:1
    }],
    tableErrorIndex:[-1,-1],
    /* service */ 
    canTable: [],
    doipTable: [],
    lpTable: [],
    linTable: [],
    running: false,
    /* hse config */
    hseConfig:{
      format:{
        nvm:[],
        ram:[]
      },
      smr:[]
    }

  },
  mutations: {
    /* hse config*/
    hseFormatLoad(state,val){
      if(val.catalog==1){
        state.hseConfig.format.nvm=val.data
      }else{
        state.hseConfig.format.ram=val.data
      }
    },
    /* uds */
    addSch(state,mode){
      state[mode+'Table'].push({
        name:'sch'+(state[mode+'Table'].length+1),
        addr:'',
        services:[]
      })
    },
    logLevel(state,level){
      state.logLevel=level
    },
    setTableError(state,index){
      state.tableErrorIndex=index
    },
    runChange(state, run) {
      state.running = run
    },
    /* connect*/
    canChange(state, connect) {
      state.canConnect = connect
    },
    doipChange(state, connect) {
      state.doipConnect = connect
    },
    linChange(state, connect) {
      state.linConnect = connect
    },
    /* doip addr table */
    doipAddrAdd(state, item) {
      state.doipAddrTable.push(item)
    },
    doipAddrDelete(state, index) {
      state.doipAddrTable.splice(index, 1)
    },
    doipAddrDeleteKey(state, key) {
      for (var i in state.doipAddrTable) {
        if (state.doipAddrTable[i].key === key) {
          state.doipAddrTable.splice(i, 1)
          break
        }
      }
      if (state.doipAddrTable.length === 0) {
        state.doipConnect = false
      }
    },
    doipAddrLoad(state, data) {
      state.doipAddrTable = data
    },
    /* can addr table */
    canAddrAdd(state, item) {
      state.canAddrTable.push(item)
    },
    canAddrDelete(state, index) {
      state.canAddrTable.splice(index, 1)
    },
    canAddrLoad(state, data) {
      state.canAddrTable = data
    },
    /* lin addr table */
    linAddrAdd(state, item) {
      state.linAddrTable.push(item)
    },
    linAddrDelete(state, index) {
      state.linAddrTable.splice(index, 1)
    },
    linAddrLoad(state, data) {
      state.linAddrTable = data
    },
    /*can table*/
    canTableLoad(state, data) {
      state.canTable = data
    },
    /*lin table*/
    linTableLoad(state, data) {
      state.linTable = data
    },
    /*doip table*/
    doipTableLoad(state, data) {
      state.doipTable = data
    },
    /*loopback table*/
    lpTableLoad(state, data) {
      state.lpTable = data
    },
  },
  actions: {

  }
})
