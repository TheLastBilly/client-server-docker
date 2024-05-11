#!/bin/bash

DOCKER_IMAGE_NAME=$(basename $(pwd))

docker build -t $(DOCKER_IMAGE_NAME) .
docker run                                  \
    -e"UID=$(id -u)"                        \
    -e "GID=$(id -g)"                       \
    -v ./src:/build                         \
    -v ./entrypoint.sh:/entrypoint.sh       \
    -e "GIT_VERSION=$(git rev-parse HEAD)"  \
                                            \
    $(DOCKER_IMAGE_NAME)
