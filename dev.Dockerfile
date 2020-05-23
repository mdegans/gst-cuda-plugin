# This Dockerfile is for making rapid changes and testing
# it copies the repo into the image

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

FROM nvcr.io/nvidia/deepstream:5.0-dp-20.04-devel

# install deps and create user
RUN apt-get update && apt-get install -y --no-install-recommends \
        git \
        libglib2.0-dev \
        libgstreamer1.0-dev \
        libgstreamer-plugins-base1.0-dev \
        python3-pip \
        python3-setuptools \
        python3-wheel \
        ninja-build \
    && pip3 install meson \
    && apt-get purge -y --autoremove \
        python3-pip \
        python3-setuptools \
        python3-wheel \
    && chmod -R o-w /opt/nvidia/deepstream/deepstream-5.0/ \
    && useradd -md /var/test -rUs /bin/false test

# fix ldconfig (dammit, Nvidia. testing testing testing!)
RUN echo "/opt/nvidia/deepstream/deepstream/lib" > /etc/ld.so.conf.d/deepstream.conf \
    && ldconfig

# copy source
WORKDIR /opt/gst-cuda-plugin/source
COPY meson.build COPYING VERSION README.md ./
COPY gst-cuda-plugin ./gst-cuda-plugin/
COPY subprojects ./subprojects/

# build and install
RUN mkdir build \
    && cd build \
    && meson --prefix=/usr .. \
    && ninja \
    && ninja test \
    && ninja install \
    && ldconfig
# TODO(mdegans): put script to run ldconfig automatically and/or search/ask for advice on best practices

# drop to user and run verbosely
USER test:test
ENV G_MESSAGES_DEBUG="all"
ENTRYPOINT [ "gst-inspect-1.0" ]
