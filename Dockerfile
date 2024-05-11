FROM debian:stable-slim

ENV TOOLCHAIN_VERSION=stable-2024.02-1
ENV BUILD_PATH=/build
ENV TOOLCHAINS_PATH=/toolchains
ENV AARCH64_TOOLCHAIN_PATH=${TOOLCHAINS_PATH}/aarch64

# Get all dependencies
ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && apt install -y    \
    build-essential                 \
    wget
RUN apt-get autoremove

# Download and install toolchain
RUN mkdir -p ${TOOLCHAINS_PATH}
WORKDIR ${TOOLCHAINS_PATH}
RUN wget -4 -O toolchain.tar.bz2 https://toolchains.bootlin.com/downloads/releases/toolchains/aarch64/tarballs/aarch64--glibc--${TOOLCHAIN_VERSION}.tar.bz2
RUN ls && pwd
RUN tar -xvf toolchain.tar.bz2
RUN rm toolchain.tar.bz2
RUN mv aarch64--glibc--${TOOLCHAIN_VERSION} ${AARCH64_TOOLCHAIN_PATH}
WORKDIR /

RUN mkdir ${BUILD_PATH}
WORKDIR ${BUILD_PATH}
RUN sh ${AARCH64_TOOLCHAIN_PATH}/relocate-sdk.sh

COPY ./entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

CMD ["/entrypoint.sh"]
