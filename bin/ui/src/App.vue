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
      <!-- <div id="test" style="width: 300px; height: 300px; margin: auto; margin-top: 150px">TESTING STUFF</div> -->
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

  protected OnMessage(msg: string, param: any) {
    console.log(`OnMessage: ${msg}`);
    switch (msg) {
      case 'startload': // start load process
        // loadingBar = document.querySelector('#loading-bar');
        // loadingLabel = document.querySelector('#loading-label');
        console.log('Starting load');
        window.SendMessage('startload', {});
        break;
      case 'postload': // load is complete
        break;
      case 'logindetails': // receiving login details to auto fill
        // document.querySelector('#inputEmail').value = param;
        // document.querySelector('#inputPassword').value = param2;
        break;
      case 'gamemode':
        this.Game.mode = param.mode;
        console.log(`Game mode recv: ${this.Game.mode}`);
        this.update++;
        break;
      case 'loadingprogress':
        if (param.val === 100) {
          window.SendMessage('loadingcomplete', {});
        }
        this.$root.$emit('progress', param.val, param.label);
        break;
    }
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
    console.log('created');
  }
  protected beforeMount() {}
  protected mounted() {
    window.SendMessage('onload', {});
  }
  protected beforeUpdate() {}
  protected updated() {}
  protected beforeDestroy() {
    this.$root.$off('message', this.OnMessage);
  }
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
#test {
  background: linear-gradient(124deg, #ff2400, #e81d1d, #e8b71d, #e3e81d, #1de840, #1ddde8, #2b1de8, #dd00f3, #dd00f3);
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
  }
}
</style>
