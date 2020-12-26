<template>
  <v-container id="waitingresponse">
    <div id="hbx-popup-center-text" class="text-center text-truncate text-h5">
      Waiting for server response...<br />
      <v-btn v-if="hasCancel" class="text-center mt-2" @click="CancelWaiting()">Cancel waiting</v-btn>
    </div>
  </v-container>
</template>
<script lang="ts">
// import { mapState } from 'vuex';
import { Component, Prop, Vue } from 'vue-property-decorator';
import { Game } from '../Game/game';

@Component
export default class WaitingResponse extends Vue {
  @Prop() private Game!: Game;
  private timeStarted: number;
  private hasCancel: boolean = false;
  private cancelTimeout!: number;
  constructor() {
    super();
    this.timeStarted = Date.now();
  }
  protected CancelWaiting() {
    window.SendMessage('cancelwaiting', {});
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
#waitingresponse {
  //position: absolute;
  //top: 0px;
  //left: 0px;
  margin-top: 35vh;
  width: 50vw;
  height: 25vh;
  background-color: rgba(97, 41, 41, 0.5);
  border-radius: 25px;
}
#hbx-popup-center-text {
  margin-top: 8vh;
}
</style>>