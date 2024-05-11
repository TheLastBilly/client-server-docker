# client-server-docker

This repository contains the source code of a simple unix sockets communications example using a client-server model. It also comes bundled with [Dockerfile](./Dockerfile) meant to allow users to cross-compile this project for the [aarch64](https://en.wikipedia.org/wiki/AArch64) architecture via the use of [Docker](https://en.wikipedia.org/wiki/Docker).

This system is made up of two applications [server](src/server.cpp) and [client/client.cpp]. We will go into detail on how to used them on the later sections.

## Project Structure
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

## Prerequisites
- gcc
- git
- make
- Docker (optional)

### Getting Prerequisites on Debian
You can install `gcc`, `git` and `make` on a Debian based system with the following command on your terminal.
```bash
sudo apt install build-essential git make
```

As of the writing of this document, it is not recommended that you install Docker from the default PPAs provided by Debian. In order to install Docker, please follow the instructions listed on the [official website](https://docs.docker.com/desktop/install/debian/).

## Clonning
Before running the code on this repository, you'll have to clone it into your system. You can do so by running the following command on your terminal.
```bash
git clone https://github.com/TheLastBilly/client-server-docker
```

## Compiling
### With Docker
Assuming you do have Docker installed on your system, you can build this reposity by simply running the [build.sh](./build.sh) script. You can do so by running the following commands from the root of this repository.

```bash
chmod +x ./build.sh
./build.sh
```

This will create the following directories on your system:
For `x86`:
- [src/build/x86/](src/build/): Build files made by the compilation process for x86.
- [src/build/x86/bin/](src/build/bin/): Contains the `server` and `client` executables for x86.
- [src/build/x86/obj/](src/build/obj/): Contains the object and dependency files created during the build process for x86.
For `aarch64`:
- [src/build/aarch64/](src/build/): Build files made by the compilation process for aarch64.
- [src/build/aarch64/bin/](src/build/bin/): Contains the `server` and `client` executables for aarch64.
- [src/build/aarch64/obj/](src/build/obj/): Contains the object and dependency files created during the build process for aarch64.

You might also noticed the following output in your terminal after running [build.sh](./build.sh):
```bash
# ...
build/x86/bin/server build/x86/bin/test.sock &
# Not necessary per say, but the socket may not
# be ready by the time the client comes online
while [ ! -S build/x86/bin/test.sock ]; do sleep 0.1; done
build/x86/bin/client build/x86/bin/test.sock VERSION
5b7b8dcc946580fb8321fceaa23bce2f58386931
build/x86/bin/client build/x86/bin/test.sock TEST
REJECTED
build/x86/bin/client build/x86/bin/test.sock QUIT
OK
```

This is the output of the `server` and `client` programs running on the docker container.

### Without Docker
Assuming you'd like to compile this project for your host system without Docker, the steps are very straightforward. Simply run the following command from the root of this repository.

```bash
make -C src/
```

This will create the following directories on your system:
- [src/build/](src/build/): Build files made by the compilation process.
- [src/build/bin/](src/build/bin/): Contains the `server` and `client` executables.
- [src/build/obj/](src/build/obj/): Contains the object and dependency files created during the build process.

You can run the files created by `make` by running the following command:
```bash
make -C src/ test
```

This should generate the following output on your terminal
```bash
# ...
make: Entering directory '/home/drevil/Documents/client-server-docker/src'
build/bin/server build/bin/test.sock &
# Not necessary per say, but the socket may not
# be ready by the time the client comes online
while [ ! -S build/bin/test.sock ]; do sleep 0.1; done
build/bin/client build/bin/test.sock VERSION
5b7b8dcc946580fb8321fceaa23bce2f58386931
build/bin/client build/bin/test.sock TEST
REJECTED
build/bin/client build/bin/test.sock QUIT
OK
make: Leaving directory '/home/drevil/Documents/client-server-docker/src'
```
