# tell the preprocessor we have config.h and build it
subdir('config_h')
config_incdir = include_directories('config_h')

# plugin sources
plugin_incdir = include_directories('include')
plugin_sources = [
  'src/gstcudaplugin.cpp',     # common plugin code
  'src/gstcudafilter.cpp',     # cudafilter Element
  'src/gstcudahash.cpp',       # cudahash Element
  'src/gstdsdistance.cpp',     # dsdistance Element
  'src/gstdsprotopayload.cpp', # dsprotopayload Element
  'src/gstpayloadbroker.cpp',  # payloadbroker Element
]

# plugin dependencies
deps = [
  dependency('cuda'),
  dependency('gstreamer-1.0'),
  dependency('gstreamer-base-1.0'),
  dependency('dsfilter-0.3'),
]

# plugin library target
gst_cuda_plugin = shared_library(
  # library name, sources
  'gstcudaplugin', plugin_sources,
  dependencies: deps,
  include_directories: [plugin_incdir, config_incdir],
  install: true,
  install_dir: plugins_install_dir,
)

# add test subdir
subdir('test')