import CopyWebpackPlugin from 'copy-webpack-plugin';
import HtmlWebpackPlugin from 'html-webpack-plugin';
import { fileURLToPath } from 'url';
import { resolve } from 'path';

const ROOT_DIR = fileURLToPath(new URL('.', import.meta.url));
function root(...args) {
  return resolve(ROOT_DIR, ...args);
}

/** @type {import('webpack').Configuration} */
export default {
  entry: root('./src/web-shell/shell.ts'),
  module: {
    rules: [
      {
        test: /\.css$/i,
        use: ['style-loader', 'css-loader'],
      },
      {
        test: /\.ts$/,
        use: 'ts-loader'
      },
    ],
  },
  resolve: {
    extensions: ['.ts', '.js', '.data', '.wasm'],
    alias: {
      "plane": root('build/wasm/bin')
    }
  },
  output: {
    filename: '[name].js',
    path: root('build/web-shell'),
  },
  optimization: {
    runtimeChunk: 'single',
  },
  plugins: [
    new HtmlWebpackPlugin({
      template: root('src/web-shell/index.html'),
    }),
    new CopyWebpackPlugin({
      patterns: [
        { from: root('build/wasm/bin/index.data'), to: 'index.data' },
        { from: root('shaders'), to: 'shaders' },
        { from: root('assets/fonts'), to: 'assets/fonts' },
        { from: root('assets/levels'), to: 'assets/levels' },
        { from: root('assets/models'), to: 'assets/models' },
      ],
    }),
  ],
};
