# gst-cuda-plugin

Is a WIP sample cuda plugin for gstreamer.

Right now it's mostly boilerplate and doesn't do much, but it has basic tests using [GstCheck](https://gstreamer.freedesktop.org/documentation/check/index.html) and [GstHarness](https://gstreamer.freedesktop.org/documentation/check/gstharness.html).

To build:
```bash
sudo apt install ninja-build libgstreamer1.0-dev libglib2.0-dev libgstreamer-plugins-base1.0-dev
git clone --branch (branch) (repo url)
pip3 install --upgrade meson
cd (repo folder)
mkdir build && cd build
meson (options) ..
ninja test
(sudo) ninja install
```

A systemwide install is not necessary if you tell meson to configure a `--prefix` like `~/.local` and set GST_PLUGIN_PATH accordingly. `ninja uninstall` can be used to uninstall.