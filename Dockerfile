IMAGE alpine:latest

ARG TOOLCHAIN_VERSION=stable-2024.02-1
ARG TOOLCHAINS_PATH=/toolchains
ARG BUILD_PATH=/build

ENV AARCH64_TOOLCHAIN_PATH=$(TOOLCHAINS_PATH)/aarch64
ENV BUILD_PATH=$(BUILD_PATH)

# Get all dependencies
RUN apk add     \
    build-base  \
    wget

# Download and install toolchain
RUN mkdir -p $(TOOLCHAINS_PATH)
WORKDIR $(TOOLCHAINS_PATH)
RUN wget -O toolchain.tar.gz2 https://toolchains.bootlin.com/downloads/releases/toolchains/aarch64/tarballs/aarch64--glibc--$(TOOLCHAIN_VERSION).tar.bz2
RUN tar -xvf toolchain.tar.bz2
RUN mv aarch64--glibc--$(TOOLCHAIN_VERSION) $(AARCH64_TOOLCHAIN_PATH)
RUN rm toolchain.tar.bz2
RUN bash aarch64/relocate-sdk.sh
ENV TOOLCHAIN_PATH=$()
WORKDIR /

COPY ./entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

RUN mkdir $(BUILD_PATH)
WORKDIR $(BUILD_PATH)

CMD ["/entrypoint.sh"]
