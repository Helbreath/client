const TerserPlugin = require('terser-webpack-plugin');
const UglifyJsPlugin = require('uglifyjs-webpack-plugin');

module.exports = {
  optimization: {
    minimizer: [
      new UglifyJsPlugin({
        uglifyOptions: {
          compress: {
            dead_code: false,
            drop_console: true,
            unused: false,
          },
          mangle: {
            reserved: ['ReceiveMessage', 'reservedName2'],
          },
        },
      }),
    ],
  },
};
