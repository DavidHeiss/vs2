<template>
  <div v-if="refreshing" class="grid justify-center h-screen w-full">
    <LoadingCircle />
  </div>
  <div v-else class="min-w-full flex flex-col">
    <div
      class="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 xl:grid-cols-4 2xl:grid-cols-5 gap-4 m-4"
    >
      <div
        class="bg-white hover:bg-gray-300 transition p-4 shadow rounded-lg flex flex-col items-center font-semibold"
      >
        <div class="text-indigo-600 text-5xl p-3">
          <span>{{
            (data?.durchschnittsgeschwindigkeit || {})[
              "average speed"
            ]?.toFixed(2)
          }}</span>
          <span class="ml-1 text-4xl">km/h</span>
        </div>
        <div class="text-gray-500 text-1xl">Durchschnittsgeschwindigkeit</div>
      </div>

      <div
        class="bg-white hover:bg-gray-300 transition p-4 shadow rounded-lg flex flex-col items-center font-semibold"
      >
        <div class="text-indigo-600 text-5xl p-3">
          <span>{{
            (
              ((data?.fuellstand || {})["current capacity"] /
                (data?.fuellstand || {})["capacity"]) *
              100
            )?.toFixed(2)
          }}</span>
          <span class="ml-1 text-4xl">%</span>
        </div>
        <div class="text-gray-500 text-1xl">Fuellstand</div>
      </div>

      <div
        class="bg-white hover:bg-gray-300 transition p-4 shadow rounded-lg flex flex-col items-center font-semibold"
      >
        <div class="text-indigo-600 text-5xl p-3">
          <span>{{ (data?.kilometerstand || {})["mileage"]?.toFixed(2) }}</span>
          <span class="ml-1 text-4xl">km</span>
        </div>
        <div class="text-gray-500 text-1xl">Kilometerstand</div>
      </div>

      <div
        class="bg-white hover:bg-gray-300 transition p-4 shadow rounded-lg flex flex-col items-center font-semibold"
      >
        <div class="text-indigo-600 text-5xl truncate max-w-full p-3">
          {{ (data?.verkehrssituation || {})["value"] }}
        </div>
        <div class="text-gray-500 text-1xl">Verkehrssituation</div>
      </div>
    </div>
    <div class="grid grid-cols-1 lg:grid-cols-2 gap-4 m-4">
      <div v-for="(item, key) in dataWithoutTime" v-bind:key="key" v-cloak>
        <h1
          class="bg-gradient-to-br from-blue-600 to-purple-600 bg-clip-text text-transparent text-4xl 2xl:text-5xl p-2 font-bold uppercase truncate"
        >
          {{ key }}
        </h1>
        <div
          class="shadow overflow-auto border-b border-gray-200 sm:rounded-lg"
        >
          <table class="min-w-full divide-y divide-gray-200">
            <thead class="bg-gray-50">
              <tr>
                <th
                  scope="col"
                  class="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider"
                >
                  Key
                </th>
                <th
                  scope="col"
                  class="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider"
                >
                  Value
                </th>
                <th
                  scope="col"
                  class="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider"
                >
                  Type
                </th>
              </tr>
            </thead>
            <tbody class="bg-white divide-y divide-gray-200">
              <tr
                v-for="(value, key) in { ...item }"
                v-bind:key="key"
                class="hover:bg-gray-300 transition"
              >
                <td class="px-6 py-4 whitespace-nowrap">{{ key }}</td>
                <td class="px-6 py-4 whitespace-nowrap">{{ value }}</td>
                <td class="px-6 py-4 whitespace-nowrap">
                  <span
                    class="px-2 inline-flex text-xs leading-5 font-semibold rounded-full bg-indigo-100 text-indigo-800 font-mono"
                  >
                    {{ typeof value }}
                  </span>
                </td>
              </tr>
            </tbody>
          </table>
        </div>
      </div>
    </div>
  </div>
  <button
    class="absolute bottom-2 right-2 mr-4 p-4  bg-indigo-700 hover:bg-indigo-500 transition rounded-full grid shadow-xl"
    v-on:click="refresh"
  >
    <span class="material-icons select-none text-white">
      refresh
    </span>
  </button>
</template>

<script lang="ts">
import { computed, defineComponent, ref, onMounted } from "vue";
import { useStore } from "vuex";
import LoadingCircle from "@/components/LoadingCircle.vue";

export default defineComponent({
  name: "Zentrale",
  setup() {
    const store = useStore();
    const refreshing = ref(false);

    onMounted(() => {
      refreshing.value = true;
      store.dispatch("getDataForZentrale").then(() => {
        refreshing.value = false;
      });
    });

    function refresh() {
      refreshing.value = true;
      store.dispatch("getDataForZentrale").then(() => {
        refreshing.value = false;
      });
    }

    const data = computed(() => {
      return store.state.zentral;
    });

    const dataWithoutTime = computed(() => {
      const keys = Object.keys(store.state.zentral).filter(
        value => value != "time"
      );
      const newObject: { [index: string]: any } = {};
      for (const value of keys) {
        newObject[value] = store.state.zentral[value];
        for (const key in newObject[value]) {
          if (key == "send" || key == "received") {
            newObject[value][key] = new Date(
              newObject[value][key]
            ).toLocaleString();
          }
        }
      }
      console.log(newObject);
      return newObject;
    });
    return { data, refreshing, dataWithoutTime, refresh };
  },
  components: { LoadingCircle }
});
</script>

Durchschnittsgeschwindigkeit
