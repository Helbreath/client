<template>
  <v-container id="mainmenu" class="text-center" style="width: 500px">
    <div class="text-truncate mt-3">Helbreath Xtreme Main Menu</div>
    <div class="mt-3">
      <v-btn class="mt-3" @click="Connect()">Connect to server</v-btn>
    </div>
  </v-container>
</template>
<script lang="ts">
// import { mapState } from 'vuex';
import { Component, Prop, Vue } from 'vue-property-decorator';
import { Game } from '../Game/game';

@Component
export default class MainMenu extends Vue {
  @Prop() private Game!: Game;
  protected mounted() {}
  protected Connect() {
    window.SendMessage('mainmenuconnect', {});
  }
}
</script>
<style lang="scss">
#mainmenu {
  margin-top: 35vh;
  width: 50vw;
  height: 25vh;
  background-color: rgba(97, 41, 41, 0.5);
  border-radius: 25px;
}
/*  background: linear-gradient(124deg, #ff2400, #e81d1d, #e8b71d, #e3e81d, #1de840, #1ddde8, #2b1de8, #dd00f3, #dd00f3);
  -webkit-animation: rainbow 18s ease infinite;
  animation: rainbow 18s ease infinite;
  background-size: 1800% 1800%;

@-webkit-keyframes rainbow {
  0% {
    background-position: 0% 82%;
  }
  50% {
    background-position: 100% 19%;
  }
  100% {
    background-position: 0% 82%;
  }
}
@keyframes rainbow {
  0% {
    background-position: 0% 82%;
  }
  50% {
    background-position: 100% 19%;
  }
  100% {
    background-position: 0% 82%;
  }
}*/
</style>
