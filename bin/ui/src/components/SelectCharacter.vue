<template>
  <v-container id="selectcharacter">
    <div id="hbx-popup-center-text" class="text-center text-truncate text-h5">Select your character</div>
    <v-btn @click="Disconnect()">Logout</v-btn>
  </v-container>
</template>
<script lang="ts">
// import { mapState } from 'vuex';
import { Component, Prop, Vue } from 'vue-property-decorator';
import { Game } from '../Game/game';

@Component
export default class SelectCharacter extends Vue {
  @Prop() private Game!: Game;
  constructor() {
    super();
  }
  protected Disconnect() {
    window.SendMessage('disconnect', {});
  }
  protected mounted() {}
  protected beforeDestroy() {}
}
</script>
<style lang="scss">
#connecting {
  margin-top: 35vh;
  width: 50vw;
  height: 25vh;
  background-color: rgba(97, 41, 41, 0.5);
  border-radius: 25px;
}
</style>
