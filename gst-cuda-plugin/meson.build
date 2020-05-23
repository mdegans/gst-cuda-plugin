# tell the preprocessor we have config.h and build it
subdir('config_h')
config_incdir = include_directories('config_h')

# plugin sources
plugin_incdir = include_directories('include')
plugin_sources = [
  'src/gstcudaplugin.cpp',  # common plugin code
  'src/gstcudafilter.cpp',  # GstCudaFilter Element
  'src/gstcudahash.cpp',    # GstCudaHash Element
  'src/gstdsdistance.cpp',    # GstDsDistance Element
]

# plugin dependencies
deps = [
  dependency('cuda'),
  dependency('gstreamer-1.0'),
  dependency('gstreamer-base-1.0'),
  libdsfilter_proj.get_variable('libdsfilter_dep'),
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