const path = require('path');

const { VueLoaderPlugin } = require('vue-loader');
const HtmlWebPackPlugin = require("html-webpack-plugin");
const MiniCssExtractPlugin = require("mini-css-extract-plugin");
const CompressionPlugin = require("compression-webpack-plugin");
const IconfontWebpackPlugin = require('iconfont-webpack-plugin');
const webpackMockServer = require("webpack-mock-server");
const ESLintPlugin = require('eslint-webpack-plugin');

module.exports = () => ({
    entry: './src/main.ts',

    resolve: {
        extensions: ['.ts', '.tsx', '.js']
    },

    output: {
        path: path.resolve(__dirname, './dist'),
    },

    module: {
        rules: [
            {
                test: /\.(js|jsx)$/,
                exclude: /node_modules/,
                use: {
                    loader: "babel-loader",
                },
            },
            {
                test: /\.vue$/,
                loader: 'vue-loader',
            },
            {
                test: /\.ts$/,
                loader: 'ts-loader',
                options: {
                    appendTsSuffixTo: [/\.vue$/]
                }
            },
            //{
            //    test: /\.css$/,
            //    use: [MiniCssExtractPlugin.loader, "css-loader"],
            //},
            {
                test: /\.html$/,
                use: [
                    {
                        loader: "html-loader",
                        options: { minimize: true },
                    },
                ],
            },
            {
                test: /\.(sass|scss|css)$/i,
                use: [
                    //"style-loader",
                    MiniCssExtractPlugin.loader,
                    // Creates `style` nodes from JS strings
                    //"style-loader",
                    // Translates CSS into CommonJS
                    "css-loader",
                    {
                        loader: 'postcss-loader',
                        options: {
                            plugins: (loader) => [
                                // Add the plugin
                                new IconfontWebpackPlugin(loader)
                            ]
                        }
                    },
                    // Compiles Sass to CSS
                    {
                        loader: "sass-loader",
                        options: {
                            // Prefer `dart-sass`
                            implementation: require("node-sass"),
                        },
                    }
                ],
            }
        ]
    },

    optimization: {
        minimize: true,
    },

    plugins: [
        new CompressionPlugin({
            algorithm: "gzip",
            compressionOptions: { level: 9 },
        }),
        new VueLoaderPlugin(),
        new HtmlWebPackPlugin({
            template: "./index.html",
            filename: "./index.html"
        }),
        new MiniCssExtractPlugin(),
        new ESLintPlugin(),
    ],

    devServer: {
        inline: true,
        hot: true,
        contentBase: __dirname,
        overlay: true,
        before: webpackMockServer.use
    }
})