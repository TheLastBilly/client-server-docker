#!/bin/sh

UID=${UID:-1000}
GID=${GID:-1000}
GIT_VERSION=${GIT_VERSION:-"NOT DEFINED"}

groupadd --gid ${GID} builder
useradd --uid ${UID} --gid ${GID} builder

su -c '\
make CXX=${TOOLCHAIN_PATH}/bin/${TOOLCHAIN_ARCH}-linux-g++ GIT_VERSION=\""${GIT_VERSION}"\" BUILD_DIR=build/${TOOLCHAIN_ARCH} && \
make BUILD_DIR=build/${HOST_ARCH} GIT_VERSION=\""${GIT_VERSION}"\" && \
make BUILD_DIR=build/${HOST_ARCH} GIT_VERSION=\""${GIT_VERSION}"\" test' builder
