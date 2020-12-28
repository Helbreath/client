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
    ReceiveMessage: (msg: string, param: string, param2: string) => void;
    emit: (data: any) => void;
    SendJsonMessage: (data: string) => void;
    log: (msg: string) => void;
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

function doOnLoad() {
  let app: Base | null = null;

  window.vueapp = app = new Base({
    store,
    vuetify,
    render: h => h(App),
  }).$mount('#app');
  window.removeEventListener('load', doOnLoad);

  window.vueapp = app;
}
export function configure() {
  // tslint:disable-next-line: no-var-requires
  window.Vue = require('vue');

  window.$ = window.jQuery = require('jquery');

  window.game = new Game();

  window.addEventListener('load', doOnLoad);
}

configure();

window.ReceiveMessage = (msg: string, param: string) => {
  // console.log(`ReceiveMessage("${msg}", "${param}")`);
  window.vueapp?.$emit('message', msg, JSON.parse(param));
};

window.SendMessage = (msg: string, data: any) => {
  window.SendJsonMessage(JSON.stringify({ msg, data }));
};
