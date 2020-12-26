//const TerserPlugin = require('terser-webpack-plugin');
//const UglifyJsPlugin = require('uglifyjs-webpack-plugin');

module.exports = {
  transpileDependencies: ['vuetify'],
  chainWebpack: config => {
    /*config.optimization.minimizer('terser').tap(args => {
      const { terserOptions } = args[0];
      terserOptions.keep_classnames = true;
      terserOptions.keep_fnames = true;
      return args;
    });*/
    /*config.optimization.minimizer('uglify').tap(args => {
      const { uglifyOptions } = args[0];
      uglifyOptions.compress.dead_code = false;
      uglifyOptions.compress.unused = false;
      return args;
    });*/
  },
  /*configureWebpack: {
    optimization: {
      minimizer: [
        new UglifyJsPlugin({
          uglifyOptions: {
            compress: {
              dead_code: false,
              drop_console: true,
              unused: false,
              keep_fnames: true,
            },
            mangle: {
              reserved: ['ReceiveMessage', 'reservedName2'],
            },
          },
        }),
      ],
    },
  },*/
};
