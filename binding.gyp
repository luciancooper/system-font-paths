{
    'targets': [{
        'target_name': '<(module_name)',
        'sources': ['src/main.cc'],
        'include_dirs': ["<!(node -p \"require('node-addon-api').include_dir\")"],
        'defines': ['NAPI_DISABLE_CPP_EXCEPTIONS'],
        'conditions': [
            ['OS=="mac"', {
                'sources': ['src/mac.mm'],
                'link_settings': {
                    'libraries': ['CoreText.framework', 'Foundation.framework']
                },
                'cflags+': ['-fvisibility=hidden'],
                'xcode_settings': {
                    'GCC_SYMBOLS_PRIVATE_EXTERN': 'YES', # -fvisibility=hidden
                }
            }],
            ['OS=="win"', {
                'sources': ['src/windows.cc'],
                'link_settings': {
                    'libraries': ['Dwrite.lib']
                },
            }],
            ['OS=="linux"', {
                'sources': ['src/linux.cc'],
                'link_settings': {
                    'libraries': ['-lfontconfig']
                }
            }],
        ],
    }, {
        'target_name': 'action_after_build',
        'type': 'none',
        'dependencies': ['<(module_name)'],
        'copies': [{
            'files': ['<(PRODUCT_DIR)/<(module_name).node'],
            'destination': '<(module_path)'
        }],
    }],
}
