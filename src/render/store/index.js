import Vue from 'vue'
import Vuex from 'vuex'

Vue.use(Vuex)

export default new Vuex.Store({
  state: {
    canConnect: false,
    doipConnect: false,
    logLevel:'info',
    /*addr*/
    canAddrTable: [],
    doipAddrTable: [],
    lpAddrTable:[{
      name:'emulate-lp',
      SA:0,
      TA:1
    }],
    tableErrorIndex:-1,
    /* service */ 
    canTable: [],
    doipTable: [],
    lpTable:[],
    running: false,

  },
  mutations: {
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
    /*can table*/
    canTableUpdate(state, index) {
      const targetRow = state.canTable.splice(index[1], 1)[0]
      state.canTable.splice(index[0], 0, targetRow)
    },
    canTableAdd(state, item) {
      item.date = new Date().getTime()
      state.canTable.push(item)
    },
    canTableDelete(state, index) {
      state.canTable.splice(index, 1)
    },
    canTableLoad(state, data) {
      state.canTable = data
    },
    canItemChange(state,val){
      val.data.date=new Date().getTime()
      Vue.set(state.canTable,val.index,val.data)
    },
    /*doip table*/
    doipTableUpdate(state, index) {
      const targetRow = state.doipTable.splice(index[1], 1)[0]
      state.doipTable.splice(index[0], 0, targetRow)
    },
    doipTableAdd(state, item) {
      item.date = new Date().getTime()
      state.doipTable.push(item)
    },
    doipTableDelete(state, index) {
      state.doipTable.splice(index, 1)
    },
    doipTableLoad(state, data) {
      state.doipTable = data
    },
    doipItemChange(state,val){
      val.data.date=new Date().getTime()
      Vue.set(state.doipTable,val.index,val.data)
    },
    /*loopback table*/
    lpTableUpdate(state, index) {
      const targetRow = state.lpTable.splice(index[1], 1)[0]
      state.lpTable.splice(index[0], 0, targetRow)
    },
    lpTableAdd(state, item) {
      item.date = new Date().getTime()
      state.lpTable.push(item)
    },
    lpTableDelete(state, index) {
      state.lpTable.splice(index, 1)
    },
    lpTableLoad(state, data) {
      state.lpTable = data
    },
    lpItemChange(state,val){
      val.data.date=new Date().getTime()
      Vue.set(state.lpTable,val.index,val.data)
    }
  },
  actions: {

  }
})
