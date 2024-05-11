#!/bin/sh

UID=${UID:-1000}
GID=${GID:-1000}
GIT_VERSION=${GIT_VERSION:-"NOT DEFINED"}

groupadd --gid ${GID} builder
useradd --uid ${UID} --gid ${GID} builder

su -c \
'make CXX=${AARCH64_TOOLCHAIN_PATH}/bin/aarch64-linux-g++ GIT_VERSION=\""${GIT_VERSION}"\" BUILD_DIR=build/aarch64 && \
make BUILD_DIR=build/x86 GIT_VERSION=\""${GIT_VERSION}"\"' builder
