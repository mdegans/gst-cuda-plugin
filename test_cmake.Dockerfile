# This Dockerfile is for making rapid changes and testing

# Copyright (C) 2020  Michael de Gans

# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
# USA

FROM nvcr.io/nvidia/deepstream:4.0.2-19.12-devel

# install deps and create user
RUN apt-get update && apt-get install -y --no-install-recommends \
    libglib2.0-dev \
    libgstreamer1.0-dev \
    libgstreamer-plugins-base1.0-dev \
    cmake \
    ninja-build \
    && cp -R /root/deepstream_sdk_v4.0.2_x86_64/sources/ /opt/nvidia/deepstream/deepstream-4.0/ \
    && useradd -md /var/test -rUs /bin/false test

# copy source
WORKDIR /opt/gst-cuda-plugin/source
COPY CMakeLists.txt COPYING README ./
COPY gst-cuda-plugin ./gst-cuda-plugin/

# build and install
RUN mkdir build \
    && cd build \
    && cmake -GNinja .. \
    && ninja \
    && ninja install

# drop to user and run verbosely
USER test:test
ENV GST_DEBUG="5"
ENV G_MESSAGES_DEBUG="all"
ENTRYPOINT [ "gst-inspect-1.0", "--plugin", "/usr/local/lib/x86_64-linux-gnu/gstreamer-1.0/libgstcudaplugin.so" ]
