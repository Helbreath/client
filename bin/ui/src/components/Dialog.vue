<template>
  <v-container style="cursor: pointer">
    <div class="text-center text-truncate mt-3">UI Dialog</div>
  </v-container>
</template>
<script lang="ts">
// import { mapState } from 'vuex';
import { Component, Prop, Vue } from 'vue-property-decorator';

@Component
export default class Dialog extends Vue {
  protected mounted() {}
  private getStatusIcon(status: number) {
    switch (status) {
      case 0:
        return 'fa-question';
      case 1:
        return 'fa-cloud-upload-alt';
      case 2:
        return 'fa-exclamation-triangle';
      case 3:
        return 'fa-times';
      case 4:
        return 'fa-check';
      case 5:
        return 'fa-power-off';
    }
    return 'fa-power-off';
  }
  private getStatusColor(status: number) {
    switch (status) {
      case 0:
        return 'teal';
      // return 'info';
      case 1:
        return 'blue darken-4';
      // return 'primary';
      case 2:
        return 'yellow';
      // return 'warning';
      case 3:
        return 'red';
      // return 'danger';
      case 4:
        return 'green';
      // return 'success';
      case 5:
        return 'red';
      // return 'danger';
    }
    return 'grey';
  }
}
</script>
<style lang="scss">
</style>
