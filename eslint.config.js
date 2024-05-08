const base = require('@lcooper/eslint-config'),
    jest = require('@lcooper/eslint-config-jest');

module.exports = [...base, jest];