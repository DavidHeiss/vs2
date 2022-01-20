import { createStore } from "vuex";
import Axios from "axios";

export default createStore({
  state: {
    zentral: {},
    durchschnittsgeschwindigkeit: {},
    fuellstand: {},
    kilometerstand: {},
    verkehrssituation: {}
  },
  mutations: {
    updateZentrale(state, data) {
      state.zentral = data;
    },
    updateDurchschnittsgeschwindigkeit(state, data) {
      state.durchschnittsgeschwindigkeit = data;
    },
    updateFuellstand(state, data) {
      state.fuellstand = data;
    },
    updateKilometerstand(state, data) {
      state.kilometerstand = data;
    },
    updateVerkehrssituation(state, data) {
      state.verkehrssituation = data;
    }
  },
  actions: {
    getDataForZentrale() {
      return new Promise((resolve, reject) => {
        Axios.get("http://localhost:8081").then(value => {
          if (value.status == 200) {
            this.commit("updateZentrale", value.data);
            resolve(value);
          } else {
            reject(value.statusText);
          }
        });
      });
    },
    getDataForDurchschnittsgeschwindigkeit() {
      return new Promise((resolve, reject) => {
        Axios.get("http://localhost:8081/durchschnittsgeschwindigkeit").then(
          value => {
            if (value.status == 200) {
              this.commit("updateDurchschnittsgeschwindigkeit", value.data);
              resolve(value);
            } else {
              reject(value.statusText);
            }
          }
        );
      });
    },
    getDataForFuellstand() {
      return new Promise((resolve, reject) => {
        Axios.get("http://localhost:8081/fuellstand").then(value => {
          if (value.status == 200) {
            this.commit("updateFuellstand", value.data);
            resolve(value);
          } else {
            reject(value.statusText);
          }
        });
      });
    },
    getDataForKilometerstand() {
      return new Promise((resolve, reject) => {
        Axios.get("http://localhost:8081/kilometerstand").then(value => {
          if (value.status == 200) {
            this.commit("updateKilometerstand", value.data);
            resolve(value);
          } else {
            reject(value.statusText);
          }
        });
      });
    },
    getDataForVerkehrssituation() {
      return new Promise((resolve, reject) => {
        Axios.get("http://localhost:8081/verkehrssituation").then(value => {
          if (value.status == 200) {
            this.commit("updateVerkehrssituation", value.data);
            resolve(value);
          } else {
            reject(value.statusText);
          }
        });
      });
    }
  }
});
