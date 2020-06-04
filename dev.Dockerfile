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

ARG DSFILTER_TAG="UNSET (use docker_build.sh to build)"
ARG AUTHOR="UNSET (use docker_build.sh to build)"
FROM ${AUTHOR}/libdsfilter:${DSFILTER_TAG}

ARG PREFIX="/usr"
ARG SRCDIR="${PREFIX}/src/gst-cuda-plugin"

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
        python3-wheel

# copy source
WORKDIR ${SRCDIR}
COPY meson.build COPYING VERSION README.md ./
COPY gst-cuda-plugin ./gst-cuda-plugin/

# build and install
RUN mkdir build \
    && cd build \
    && meson --prefix=${PREFIX} .. \
    && ninja \
    && ninja test \
    && ninja install \
    && ldconfig

# run verbosely
ENV G_MESSAGES_DEBUG="all"
ENTRYPOINT [ "gst-inspect-1.0" ]
