<template>
  <v-container id="loading">
    <v-progress-linear color="red lighten-2" :value="progress" stream></v-progress-linear>
    <div class="text-center text-truncate mt-3">{{ label }}</div>
  </v-container>
</template>
<script lang="ts">
// import { mapState } from 'vuex';
import { Component, Prop, Vue } from 'vue-property-decorator';
import { Game } from '../Game/game';

@Component
export default class Loading extends Vue {
  @Prop() private Game!: Game;
  private progress: number = 0;
  private label: string = '';
  protected mounted() {
    this.$root.$on('progress', this.OnProgress);
  }
  protected beforeDestroy() {
    this.$root.$off('progress', this.OnProgress);
  }
  protected OnProgress(val: number, label: string) {
    this.progress = val;
    this.label = label;
  }
}
</script>
<style lang="scss">
#loading {
  margin-top: 45vh;
  width: 50vw;
  height: 25vh;
}
</style>
