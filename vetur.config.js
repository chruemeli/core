module.exports = {
    projects: [
        {
            root: "./webapp",
            package: './package.json',
            tsconfig: './tsconfig.json',
            globalComponents: [
                './src/views/**/*.vue'
            ]
        }
    ]
}