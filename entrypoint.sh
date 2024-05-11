#!/bin/bash

useradd -u $(UID) -g $(GID) builder

cd $(BUILD_PATH)
su - builder
make CXX=$(AARCG64_TOOLCHAIN_PATH)/bin/aarch64-linux-g++
