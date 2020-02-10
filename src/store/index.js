import Vue from 'vue'
import Vuex from 'vuex'

Vue.use(Vuex)

export default new Vuex.Store({
  state: {
    canConnect: false,
    canTpMapTable: [],
    udsTable: [],
    running: false
  },
  mutations: {
    runChange (state, run) {
      state.running = run
    },
    canChange (state, connect) {
      state.canConnect = connect
    },
    canTpMapAdd (state, item) {
      state.canTpMapTable.push(item)
    },
    canTpMapDelete (state, index) {
      state.canTpMapTable.splice(index, 1)
    },
    canTpMapLoad (state, data) {
      state.canTpMapTable = data
    },
    udsTableUpdate (state, index) {
      const targetRow = state.udsTable.splice(index[1], 1)[0]
      state.udsTable.splice(index[0], 0, targetRow)
    },
    udsTableAdd (state, item) {
      item.date = new Date().getTime()
      state.udsTable.push(item)
    },
    udsTableDelete (state, index) {
      state.udsTable.splice(index, 1)
    },
    udsTableLoad (state, data) {
      state.udsTable = data
    }
  },
  actions: {

  }
})
