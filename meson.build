# source of truth for package info 
# (changing these changes config_meson.h and many other things)
project('gst-cuda-plugin', ['cpp'],
  version : '0.1',
  license : 'LGPL',
)
gst_api_version = '1.0'
package_name = 'gstcudaplugin'
package_long_name = 'GStreamer CUDA plugin'
package_description = package_long_name
package_version = meson.project_version()
package_author = 'Michael de Gans'
package_email = 'michael.john.degans@gmail.com'
package_uri = 'https://github.com/mdegans/gst-cuda-plugin'

# get the cpp compiler
cc = meson.get_compiler('cpp')

# calculate and print the plugin install dir
plugins_install_dir = join_paths(get_option('libdir'), 'gstreamer-1.0')
message('gst plugin dir: ' + plugins_install_dir)

# include subdirs
subdir('gst-cuda-plugin')
