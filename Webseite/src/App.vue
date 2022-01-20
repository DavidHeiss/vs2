<template>
  <div class="flex flex-col">
    <header class="shadow">
      <nav class="box-border bg-indigo-700 flex flex-wrap items-center">
        <router-link
          to="/"
          class="p-2 m-2 hover:bg-indigo-600 rounded-md transition text-white text-sm font-medium"
          >Zentrale</router-link
        >
        <router-link
          to="/Verkehrssituation"
          class="p-2 m-2 hover:bg-indigo-600 rounded-md transition text-white text-sm font-medium"
          >Verkehrssituation</router-link
        >
        <router-link
          to="/Durchschnittsgeschwindigkeit"
          class="p-2 m-2 hover:bg-indigo-600 rounded-md transition text-white text-sm font-medium"
          >Durchschnittsgeschwindigkeit</router-link
        >
        <router-link
          to="/Kilometerstand"
          class="p-2 m-2 hover:bg-indigo-600 rounded-md transition text-white text-sm font-medium"
          >Kilometerstand</router-link
        >
        <router-link
          to="/Fuellstand"
          class="p-2 m-2 hover:bg-indigo-600 rounded-md transition text-white text-sm font-medium"
          >Fuellstand</router-link
        >
        <span class="flex-grow" />
      </nav>
    </header>
    <main class="flex-grow bg-gray-100">
      <router-view />
    </main>
    <footer></footer>
  </div>
</template>
