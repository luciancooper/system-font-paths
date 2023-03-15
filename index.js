const binary = require('@mapbox/node-pre-gyp'),
    path = require('path'),
    // eslint-disable-next-line import/no-dynamic-require
    { systemFontPaths } = require(binary.find(path.resolve(path.join(__dirname, './package.json'))));

module.exports = () => new Promise((resolve, reject) => {
    systemFontPaths(resolve);
});