# system-font-paths

[![npm](https://img.shields.io/npm/v/system-font-paths?logo=npm&style=for-the-badge)](https://www.npmjs.com/package/system-font-paths)
[![ci](https://img.shields.io/github/actions/workflow/status/luciancooper/system-font-paths/ci.yml?logo=github&style=for-the-badge)](https://github.com/luciancooper/system-font-paths/actions/workflows/ci.yml)
[![license](https://img.shields.io/github/license/luciancooper/system-font-paths?color=yellow&style=for-the-badge)](#license)

A C++ module for Node.js that lists all locally installed system font files.

## About

This package exports a single async function that returns an array containing the file paths of all locally installed fonts. It is a slimmed down version of [`fontmanager-redux`](https://github.com/Eugeny/fontmanager-redux).

**Platforms**

* Mac OS X 10.5 and later supported via [CoreText](https://developer.apple.com/documentation/coretext)
* Windows 7 and later supported via [DirectWrite](https://learn.microsoft.com/en-us/windows/win32/directwrite/direct-write-portal)
* Linux supported via [fontconfig](http://www.freedesktop.org/software/fontconfig)

## Installation

Install with `npm`:

```bash
npm install system-font-paths
```

Or with `yarn`:

```bash
yarn add system-font-paths
```

Prebuilt binaries for node 16, 18, 20, and 22 are provided via [`node-pre-gyp`](https://github.com/mapbox/node-pre-gyp) for the following platforms:

- macOS x64 and arm64
- Windows x64 and x86
- Linux x64 (Ubuntu)

## API

This package exports a single asynchronous function that promises an array of font file path strings:

```js
import systemFontPaths from 'system-font-paths';

systemFontPaths().then((fontPaths) => { ... });
```

## Acknowledgements

This project is based heavily on the `fontmanager-redux` and `font-manager` packages:

 * [`fontmanager-redux`](https://github.com/Eugeny/fontmanager-redux)
 * [`font-manager`](https://github.com/foliojs/font-manager)

## License

[MIT](LICENSE)
