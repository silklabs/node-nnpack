{
  'targets': [
    {
      'target_name': 'nnpack',
      'sources': [
        'src/nnpack.cpp'
      ],
      'include_dirs': [
        '<!(node -e "require(\'nan\')")',
        '<!(echo $NNPACK_ROOT/include)',
        '<!(echo $NNPACK_ROOT/third-party/pthreadpool/include)'
      ],
      'libraries': [
        '-L<!(echo "$NNPACK_ROOT/lib")',
	'-lnnpack'
      ],
      'ldflags': [
      ],
      'cflags_cc': [
        '-std=c++11',
        '-fexceptions',
      ],
      'defines': [
      ],
      'dependencies': [
      ],
      'conditions': [
        ['OS=="win"', {
          'dependencies': [
          ]
        }],
        ['OS=="mac"', {
	  'defines': [
          ],
	  'include_dirs': [
	  ],
	  'libraries': [
	  ],
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'MACOSX_DEPLOYMENT_TARGET': '10.11',
            'OTHER_LDFLAGS': [
            ],
            'OTHER_CFLAGS': [
            ]
          },
          'libraries': [
          ],
        }]
      ]
    }
  ]
}