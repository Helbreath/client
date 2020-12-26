<template>
  <v-app id="hbxapp">
    <!--<v-app-bar app color="primary" dark>
      <div class="d-flex align-center">Helbreath Xtreme</div>
    </v-app-bar>-->

    <v-main :key="update">
      <MainMenu v-if="this.Game.mode === 'mainmenu'" :Game="this.Game" />
      <Connecting v-else-if="this.Game.mode === 'connecting'" :Game="this.Game" />
      <WaitingResponse v-else-if="this.Game.mode === 'waitingresponse'" :Game="this.Game" />
      <Loading v-else-if="this.Game.mode === 'loading'" :Game="this.Game" />
      <SelectCharacter v-else-if="this.Game.mode === 'selectcharacter'" :Game="this.Game" />
      <!--<Dialog />-->
    </v-main>
  </v-app>
</template>

<script lang="ts">
import { Component, Prop, Vue } from 'vue-property-decorator';
import { Game } from './Game/game';
import Dialog from './components/Dialog.vue';
import MainMenu from './components/MainMenu.vue';
import WaitingResponse from './components/WaitingResponse.vue';
import Connecting from './components/Connecting.vue';
import Loading from './components/Loading.vue';
import SelectCharacter from './components/SelectCharacter.vue';

@Component({
  components: {
    Dialog,
    MainMenu,
    WaitingResponse,
    Connecting,
    Loading,
    SelectCharacter,
  },
})
export default class App extends Vue {
  @Prop() private source!: string;

  private Game: Game;
  private update: number = 0;

  constructor() {
    super();
    this.Game = new Game();
  }

  protected OnMessage(msg: string, param: string, param2: string) {
    console.log(`OnMessage: ${msg}`);
    switch (msg) {
      case 'startload': // start load process
        // loadingBar = document.querySelector('#loading-bar');
        // loadingLabel = document.querySelector('#loading-label');
        console.log('Starting load');
        window.StartLoading();
        break;
      case 'postload': // load is complete
        break;
      case 'logindetails': // receiving login details to auto fill
        // document.querySelector('#inputEmail').value = param;
        // document.querySelector('#inputPassword').value = param2;
        break;
    }
  }

  protected UpdateGameMode(mode: string) {
    this.Game.mode = mode;
    console.log(`Game mode recv: ${this.Game.mode}`);
    this.update++;
  }

  // protected data() {}

  protected Exit() {
    window.vueapp?.$emit('exit');
    // window.SendMessage('exit');
  }

  protected Login() {
    if (window.game.mode !== 'mainmenu') return;
    const account = 'a';
    const password = 'b';
    this.$root.$emit('login', { account, password });
    // if (gameMode != 'mainmenu') return;
    // document.querySelector('#spinner').style.visibility = 'visible';
    // let account = document.querySelector('#inputEmail').value;
    // let password = document.querySelector('#inputPassword').value;
    // window.SendMessage('login', { account, password });
  }

  protected SetResolution(x: number, y: number) {
    window.vueapp?.$emit('resolution', { x, y });
    // SendMessage('resolution', { x, y });
  }

  protected beforeCreate() {}
  protected created() {
    this.$vuetify.theme.dark = true;
    this.$root.$on('message', this.OnMessage);
    this.$root.$on('gamemode', this.UpdateGameMode);
    console.log('created');
  }
  protected beforeMount() {}
  protected mounted() {
    window.SendMessage('onload', {});
    window.GetGameMode();
  }
  protected beforeUpdate() {}
  protected updated() {}
  protected beforeDestroy() {}
  protected destroyed() {}
}
</script>

<style lang="scss">
#hbxapp {
  width: calc(100vw);
  height: calc(100vh);

  //position: absolute;
  //top: 0px;
  //left: 0px;

  width: 100vw;
  max-height: 100vh;
  max-width: 100vw;
  //margin-left: 25vw;
  background: rgba(0, 0, 0, 0);
  //padding-bottom: 8px;
  -webkit-user-select: none;
  user-select: none;
}
</style>
