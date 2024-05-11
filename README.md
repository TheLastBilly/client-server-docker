# client-server-docker

This repository contains the source code of a simple unix sockets communications example using a client-server model. It also comes bundled with [Dockerfile](./Dockerfile) meant to allow users to cross-compile this project for the [aarch64](https://en.wikipedia.org/wiki/AArch64) architecture via the use of [Docker](https://en.wikipedia.org/wiki/Docker).

This system is made up of two applications [server](src/server.cpp) and [client/client.cpp]. We will go into detail on how to used them on the later sections.

## Project structure
- [build.sh](./build.sh): Build script use to compile (and cross-compile) the project via a Docker container. Will also create/update the Docker image.
- [Dockerfile](./Dockerfile): [Dockerfile](https://docs.docker.com/reference/dockerfile/) for the project. Defines the base image, environment variables, and packages bundled with the docker image.
- [entrypoint.sh](./entrypoint.sh): Entrypoint script for the Docker container. This is the first user level script ran by the container once launched.
- [README.md](./README.md): This file you are reading at the moment.
- [src/](./src/): Source directory. Source, header and Make files are located in here
- [src/Makefile](src/Makefile): [Makefile](https://makefiletutorial.com/) for this application. Defines the compilation rules and paths used to build the project.
- [src/socket.hpp](src/socket.hpp): Declarations for the Socket::Connection, Socket::Server and Socket::Client classes.
- [src/utils.hpp](src/utils.hpp): Miscellaneous utilities used throught the program.
- [src/socket.cpp](src/socket.cpp): Definitions for the ocket::Connection, Socket::Server and Socket::Client classes.
- [src/server.cpp](src/server.cpp): `server` application code.
- [src/client.cpp](src/client.cpp): `client` application code.
