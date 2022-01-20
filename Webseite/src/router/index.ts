import { createRouter, createWebHashHistory, RouteRecordRaw } from "vue-router";
import Zentrale from "../views/Zentrale.vue";
import Verkehrssituation from "../views/Verkehrssituation.vue";
import Fuellstand from "../views/Fuellstand.vue";
import Durchschnittsgeschwindigkeit from "../views/Durchschnittsgeschwindigkeit.vue";
import Kilometerstand from "../views/Kilometerstand.vue";

const routes: Array<RouteRecordRaw> = [
  {
    path: "/",
    name: "Zentrale",
    component: Zentrale
  },
  {
    path: "/Verkehrssituation",
    name: "Verkehrssituation",
    component: Verkehrssituation
  },
  {
    path: "/Fuellstand",
    name: "Fuellstand",
    component: Fuellstand
  },
  {
    path: "/Durchschnittsgeschwindigkeit",
    name: "Durchschnittsgeschwindigkeit",
    component: Durchschnittsgeschwindigkeit
  },
  {
    path: "/Kilometerstand",
    name: "Kilometerstand",
    component: Kilometerstand
  }
];

const router = createRouter({
  history: createWebHashHistory(),
  routes
});

export default router;
