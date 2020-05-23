# This Dockerfile is for production
# it clones from master and deletes the source
# use this when size matters most

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

ARG USERNAME="cudaplugin"
ARG GIT_REPO="https://github.com/mdegans/gst-cuda-plugin.git"
ARG BRANCH="master"

# install build deps, build, clean up, add user
RUN apt-get update && apt-get install -y --no-install-recommends \
        git \
        python3-pip \
        python3-setuptools \
        python3-wheel \
        ninja-build \
    && pip3 install meson \
    && chmod -R o-w /opt/nvidia/deepstream/deepstream-5.0/ \
    && echo "/opt/nvidia/deepstream/deepstream/lib" > /etc/ld.so.conf.d/deepstream.conf \
    && ldconfig \
    && git clone --depth 1 --branch ${BRANCH} ${GIT_REPO} \
    && cd gst-cuda-plugin \
    && mkdir build \
    && cd build \
    && meson --prefix=/usr .. \
    && ninja \
    && ninja test \
    && ninja install \
    && ldconfig \
    && apt-get purge -y --autoremove \
        git \
        python3-pip \
        python3-setuptools \
        python3-wheel \
        ninja-build \
    && rm -rf /var/lib/apt/lists/* \
    && useradd -md /var/${USERNAME} -rUs /bin/false ${USERNAME}

# drop to user
USER ${USERNAME}:${USERNAME}
