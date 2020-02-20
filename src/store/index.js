import Vue from 'vue'
import Vuex from 'vuex'

Vue.use(Vuex)

export default new Vuex.Store({
  state: {
    canConnect: false,
    canTpMapTable: [],
    canTable: [],
    doipTable: [],
    doipAddrTable: [],
    running: false,
    ipConnect: false,

  },
  mutations: {
    runChange (state, run) {
      state.running = run
    },
    canChange (state, connect) {
      state.canConnect = connect
    },
    ipChange (state, connect) {
      state.ipConnect = connect
    },
    /* doip addr table */
    doipAddrAdd (state, item) {
      state.doipAddrTable.push(item)
    },
    doipAddrDelete (state, index) {
      state.doipAddrTable.splice(index, 1)
    },
    doipAddrMapLoad (state, data) {
      state.doipAddrTable = data
    },
    /* can addr table */
    canTpMapAdd (state, item) {
      state.canTpMapTable.push(item)
    },
    canTpMapDelete (state, index) {
      state.canTpMapTable.splice(index, 1)
    },
    canTpMapLoad (state, data) {
      state.canTpMapTable = data
    },
    /*can table*/
    canTableUpdate (state, index) {
      const targetRow = state.canTable.splice(index[1], 1)[0]
      state.canTable.splice(index[0], 0, targetRow)
    },
    canTableAdd (state, item) {
      item.date = new Date().getTime()
      state.canTable.push(item)
    },
    canTableDelete (state, index) {
      state.canTable.splice(index, 1)
    },
    canTableLoad (state, data) {
      state.canTable = data
    },
    /*doip table*/
    doipTableUpdate (state, index) {
      const targetRow = state.doipTable.splice(index[1], 1)[0]
      state.doipTable.splice(index[0], 0, targetRow)
    },
    doipTableAdd (state, item) {
      item.date = new Date().getTime()
      state.doipTable.push(item)
    },
    doipTableDelete (state, index) {
      state.doipTable.splice(index, 1)
    },
    doipTableLoad (state, data) {
      state.doipTable = data
    }
  },
  actions: {

  }
})
