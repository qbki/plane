import CopyWebpackPlugin from 'copy-webpack-plugin';
import HtmlWebpackPlugin from 'html-webpack-plugin';
import { fileURLToPath } from 'url';
import { resolve } from 'path';


const ROOT_DIR = fileURLToPath(new URL('.', import.meta.url));
function root(...args) {
  return resolve(ROOT_DIR, ...args);
}

/**
 * @template T
 * @param mode {import('webpack').Configuration["mode"]}
 * @param dev_value {T}
 * @param other_value {T}
 * @return {T}
 */
function dev_select(mode, dev_value, other_value) {
  return mode == "development" ? dev_value : other_value;
}

/**
 * @param config {import('webpack').Configuration}
 */
export default (_, config) => ({
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
      "plane": root('build/wasm32-emscripten-release/bin')
    }
  },
  output: {
    filename: '[name].js',
    path: root('build/web-shell'),
    clean: true,
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
        { from: root('shaders'), to: 'shaders' },
        ...dev_select(
          config.mode,
          [
            { from: root('assets/fonts'), to: 'assets/fonts' },
            { from: root('assets/levels'), to: 'assets/levels' },
            { from: root('assets/models'), to: 'assets/models' },
            { from: root('assets/sound'), to: 'assets/sound' },
          ],
          [],
        )
      ],
    }),
  ],
});
