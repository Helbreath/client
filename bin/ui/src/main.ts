import { Component, Prop, Vue } from 'vue-property-decorator';
import App from './App.vue';
// import router from './router';
import store from './store';
import vuetify from './plugins/vuetify';
import 'roboto-fontface/css/roboto/roboto-fontface.css';
import '@mdi/font/css/materialdesignicons.css';
import { Game } from './Game/game';
import jquery from 'jquery';

Vue.config.productionTip = false;

declare module 'vue/types/vue' {
  interface Vue {
    /*$store: Store<any>;
      $router: VueRouter;
      $route: Route;*/
    doSuccess(message: string): void;
    doError(message: string): void;
  }
}

declare global {
  interface Window {
    jQuery: JQueryStatic;
    $: JQueryStatic;
    _: any;
    vueapp: Base | null;
    game: Game;
    SendMessage: (msg: string, data: any) => void;
    StartLoading: () => void;
    GetGameMode: () => void;
    ReceiveMessage: (msg: string, param: string, param2: string) => void;
    UpdateGameMode: (val: string) => void;
    LoadingProgress: (val: number, label: string) => void;
  }
}

@Component
class Base extends Vue {
  private drawer: any = null;
  private timeoutTracker: any = null;

  @Prop() private source!: string;

  public doError(message: string) {
    clearTimeout(this.timeoutTracker);
    const err = $('.status-overlay');
    const errtext = $('#aegis-status');
    err.removeClass('hidden').addClass('visible');
    errtext.text(message);
    errtext.removeClass('alert-success').addClass('alert-danger');
    this.timeoutTracker = setTimeout(() => {
      err.removeClass('visible').addClass('hidden');
    }, 3000);
  }
  public doSuccess(message: string) {
    clearTimeout(this.timeoutTracker);
    const err = $('.status-overlay');
    const errtext = $('#aegis-status');
    err.removeClass('hidden').addClass('visible');
    errtext.text(message);
    errtext.removeClass('alert-danger').addClass('alert-success');
    this.timeoutTracker = setTimeout(() => {
      err.removeClass('visible').addClass('hidden');
    }, 3000);
  }

  protected beforeCreate() {}
  protected created() {}
  protected beforeMount() {}
  protected mounted() {}
  protected beforeUpdate() {}
  protected updated() {}
  protected beforeDestroy() {}
  protected destroyed() {}
  private avatar() {
    return this.$store.state.Account.data.avatar;
  }
  private credit() {
    return this.$store.state.Account.data.credit;
  }
  private username() {
    return this.$store.state.Account.data.username;
  }
  private auth() {
    return this.$store.state.Account.data.auth;
  }
}

export function configure() {
  // tslint:disable-next-line: no-var-requires
  window.Vue = require('vue');

  window.$ = window.jQuery = require('jquery');

  let app: Base | null = null;

  window.vueapp = app;

  window.game = new Game();

  window.addEventListener('load', () => {
    // const app2 = new Base();
    window.vueapp = app = new Base({
      // router,
      store,
      vuetify,
      render: h => h(App),
    }).$mount('#app');
  });
}

configure();

function ReceiveMessage(msg: string, param: string, param2: string) {
  window.vueapp?.$emit('message', msg, param, param2);
}

function UpdateGameMode(val: string) {
  window.vueapp?.$emit('gamemode', val);
  // this.Game.mode = val;
  // console.log(`Game mode recv: ${this.Game.mode}`);
  /*switch (val) {
    case 'mainmenu': // main menu
      document.querySelector('#loading-bar-parent').style.visibility = 'hidden';
      document.querySelector('#login').style.visibility = 'visible';
      break;
    case 'waitingresponse': // logging in
      document.querySelector('#loading-bar-parent').style.visibility = 'hidden';
      document.querySelector('#login').style.visibility = 'hidden';
      break;
  }*/
  /*console.log('hiding all ui elements');
  for (let f of document.querySelectorAll(`.hbx-ui`)) {
    console.log(`hiding ${f.id}`);
    f.style.visibility = 'hidden';
  }
  console.log(`Showing #${this.Game.mode}`);
  document.querySelector(`#${this.Game.mode}`).style.visibility = 'visible';
  document.querySelector(`#mainmenu`).style.visibility = 'visible';*/
}

function LoadingProgress(val: number, label: string) {
  window.vueapp?.$emit('progress', val, label);
  // console.log(`Loading progress ${val} - ${label}`);
  // loadingBar.style.width = val + '%';
  // loadingLabel.innerHTML = label;
  if (val === 100) {
    // document.querySelector('#loading-bar-parent').style.visibility = 'hidden';
    // document.querySelector('#login').style.visibility = 'visible';
    window.vueapp?.$emit('load complete');
    // SendMessage('load complete');
  }
}

window.UpdateGameMode = UpdateGameMode;
window.ReceiveMessage = ReceiveMessage;
window.LoadingProgress = LoadingProgress;
