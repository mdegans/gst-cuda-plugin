
# a list of dicts describing tests
tests = [
  {
    'description': 'Test cudafilter element using GstCheck ',
    'filename': 'test_gstcudafilter',
    'sources': ['test_gstcudafilter.cpp'],
  },
  {
    'description': 'Test cudahash element using GstCheck   ',
    'filename': 'test_gstcudahash',
    'sources': ['test_gstcudahash.cpp'],
  },
  {
    'description': 'Test dsdistance element using GstCheck   ',
    'filename': 'test_gstdsdistance',
    'sources': ['test_gstdsdistance.cpp'],
  },
]

# build and run tests (on ninja test)
foreach t: tests
  exe = executable(t['filename'], t['sources'],
    dependencies: [deps, dependency('gstreamer-check-1.0')],
    link_with: gst_cuda_plugin,
    include_directories: plugin_incdir,
  )
  test(t['description'], exe,
    is_parallel: false,
    # TODO(mdegans): research if there is a dedicated parent directory function
    env: [
      'GST_DEBUG=4',
      'GST_PLUGIN_PATH=' + meson.current_build_dir() + '/../',
      'GST_REGISTRY_UPDATE=yes',
    ],
  )
endforeach