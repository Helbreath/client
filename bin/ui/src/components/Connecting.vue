<template>
  <v-container id="connecting">
    <div id="hbx-popup-center-text" class="text-center text-truncate text-h5">
      Connecting...<br />
      <v-btn v-if="hasCancel" class="text-center mt-2" @click="CancelConnect()">Cancel connecting</v-btn>
    </div>
  </v-container>
</template>
<script lang="ts">
// import { mapState } from 'vuex';
import { Component, Prop, Vue } from 'vue-property-decorator';
import { Game } from '../Game/game';

@Component
export default class Connecting extends Vue {
  @Prop() private Game!: Game;
  private timeStarted: number;
  private hasCancel: boolean = false;
  private cancelTimeout!: number;
  constructor() {
    super();
    this.timeStarted = Date.now();
  }
  protected CancelConnect() {
    window.SendMessage('cancelconnect', {});
  }
  protected AddCancel() {
    this.hasCancel = true;
    this.cancelTimeout = 0;
  }
  protected mounted() {
    this.cancelTimeout = setTimeout(this.AddCancel, 5000);
  }
  protected beforeDestroy() {
    if (this.cancelTimeout) clearTimeout(this.cancelTimeout);
  }
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
