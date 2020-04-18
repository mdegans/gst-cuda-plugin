# tell the preprocessor we have config.h and build it
subdir('config_h')
config_incdir = include_directories('config_h')

# plugin sources
plugin_incdir = include_directories('include')
plugin_sources = [
  'src/gstcudaplugin.cpp',  # common plugin code
  'src/gstcudafilter.cpp',  # GstCudaFilter Element
]

# plugin dependencies
deps = [
  dependency('gstreamer-1.0'),
  dependency('gstreamer-base-1.0'),
  dependency('dsfilter', version: '>=0.0.1')
]

# plugin library target
gst_cuda_plugin = shared_library(
  # library name, sources
  'gstcudaplugin', plugin_sources,
  dependencies: [deps, dependency('cuda')],
  include_directories: [plugin_incdir, config_incdir],
  install: true,
  install_dir: plugins_install_dir,
)

# plugin pkg-config (.po) target
pkg = import('pkgconfig')
pkg.generate(gst_cuda_plugin,
  description: package_description,
  filebase: gst_cuda_plugin.name(),
  requires: deps,
  url: package_uri,
  version: package_version,
)