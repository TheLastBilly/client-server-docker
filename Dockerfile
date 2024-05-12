FROM debian:bookworm-slim
MAINTAINER TheLastBilly drevil@jmq.sh

# Architecture and version settings
ENV HOST_ARCH=x86
ENV TOOLCHAIN_ARCH=aarch64
ENV TOOLCHAIN_VERSION=stable-2024.02-1

# Paths
ENV BUILD_PATH=/build
ENV TOOLCHAINS_PATH=/toolchains

# Toolchain settings
ENV TOOLCHAIN_PATH=${TOOLCHAINS_PATH}/${TOOLCHAIN_ARCH}
ENV TOOLCHAIN_LIBC=glibc
ENV TOOLCHAIN_NAME=${TOOLCHAIN_ARCH}--${TOOLCHAIN_LIBC}--${TOOLCHAIN_VERSION}
ENV TOOLCHAIN_TAR=${TOOLCHAIN_NAME}.tar.bz2

# Get all dependencies
ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && apt install -y    \
    build-essential                 \
    wget
RUN apt-get autoremove

# Download and install toolchain
RUN mkdir -p ${TOOLCHAINS_PATH}
WORKDIR ${TOOLCHAINS_PATH}
RUN wget -4 -O toolchain.tar.bz2 https://toolchains.bootlin.com/downloads/releases/toolchains/${TOOLCHAIN_ARCH}/tarballs/${TOOLCHAIN_TAR}
RUN ls && pwd
RUN tar -xvf toolchain.tar.bz2
RUN rm toolchain.tar.bz2
RUN mv ${TOOLCHAIN_NAME} ${TOOLCHAIN_PATH}
WORKDIR /

# Setup build path
RUN mkdir ${BUILD_PATH}
WORKDIR ${BUILD_PATH}
RUN sh ${TOOLCHAIN_PATH}/relocate-sdk.sh

# Setup entry point
COPY ./entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

CMD ["/entrypoint.sh"]
