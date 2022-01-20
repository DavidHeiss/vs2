<template>
  <div v-if="refreshing" class="grid h-screen w-full">
    <LoadingCircle />
  </div>
  <div v-else class="grid gap-4 m-4">
    <div class="shadow overflow-auto border-b border-gray-200 sm:rounded-lg">
      <table class="min-w-full divide-y divide-gray-200">
        <thead class="bg-gray-50">
          <tr>
            <th
              v-for="(value, key) in (data?.data || [0])[0]"
              v-bind:key="key"
              scope="col"
              class="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider"
            >
              {{ key }}
            </th>
          </tr>
        </thead>
        <tbody class="bg-white divide-y divide-gray-200">
          <tr
            v-for="(values, index) in data?.data"
            v-bind:key="index"
            class="hover:bg-gray-300 transition cursor-pointer"
            v-on:click="open(values.id)"
          >
            <td
              v-for="(value, key) in values"
              v-bind:key="key"
              class="px-6 py-4 whitespace-nowrap"
            >
              {{ value }}
            </td>
          </tr>
        </tbody>
      </table>
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
  name: "Durchschnittsgeschwindigkeit",
  setup() {
    const store = useStore();
    const refreshing = ref(false);

    function refresh() {
      refreshing.value = true;
      store.dispatch("getDataForDurchschnittsgeschwindigkeit").then(() => {
        refreshing.value = false;
      });
    }

    onMounted(() => {
      refresh();
    });

    function open(id: number): void {
      window.open(`http://localhost:8081/durchschnittsgeschwindigkeit/${id}`);
    }

    const data = computed(() => {
      return store.state.durchschnittsgeschwindigkeit;
    });

    const dataWithoutTime = computed(() => {
      const keys = Object.keys(store.state.durchschnittsgeschwindigkeit).filter(
        value => value != "time"
      );
      const newObject: { [index: string]: any } = {};
      for (const value of keys) {
        newObject[value] = store.state.durchschnittsgeschwindigkeit[value];
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
    return { data, refreshing, dataWithoutTime, refresh, open };
  },
  components: { LoadingCircle }
});
</script>
