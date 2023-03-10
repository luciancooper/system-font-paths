const { systemFontPaths } = require('./build/Release/system-font-paths.node');

module.exports = () => new Promise((resolve, reject) => {
    systemFontPaths(resolve);
});