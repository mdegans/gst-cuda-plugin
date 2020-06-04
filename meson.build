# source of truth for package info 
# (changing these changes config_meson.h and many other things)
project('gst-cuda-plugin', ['cpp'],
  version: run_command(
    'head', '-n', '1', files('VERSION'),
  ).stdout().strip(),
  meson_version: '>= 0.54.0',
  license : 'LGPL',
)

gst_api_version = '1.0'
package_name = 'gstcudaplugin'
package_long_name = package_name + '-' + gst_api_version
package_description = 'CUDA Plugin collection for GStreamer and DeepStream.'
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
