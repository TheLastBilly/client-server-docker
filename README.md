# client-server-docker

This repository contains the source code of a simple UNIX sockets communications example using a client-server model. It also comes bundled with [Dockerfile](./Dockerfile) meant to allow users to cross-compile this project for the [aarch64](https://en.wikipedia.org/wiki/AArch64) architecture via the use of [Docker](https://en.wikipedia.org/wiki/Docker).

This system is made up of two applications [server](src/server.cpp) and [client](src/client.cpp). We will go into detail on how to used them on the later sections.

## Project Structure
- [build.sh](./build.sh): Build script use to compile (and cross-compile) the project via a Docker container. Will also create/update the Docker image.
- [Dockerfile](./Dockerfile): [Dockerfile](https://docs.docker.com/reference/dockerfile/) for the project. Defines the base image, environment variables, and packages bundled with the Docker image.
- [entrypoint.sh](./entrypoint.sh): Entry point script for the Docker container. This is the first user level script ran by the container once launched.
- [README.md](./README.md): This file you are reading at the moment.
- [src/](./src/): Source directory. Source, header and Make files are located in here
- [src/Makefile](src/Makefile): [Makefile](https://makefiletutorial.com/) for this application. Defines the compilation rules and paths used to build the project.
- [src/socket.hpp](src/socket.hpp): Declarations for the Socket::Connection, Socket::Server and Socket::Client classes.
- [src/utils.hpp](src/utils.hpp): Miscellaneous utilities used throughout the program.
- [src/socket.cpp](src/socket.cpp): Definitions for the Socket::Connection, Socket::Server and Socket::Client classes.
- [src/server.cpp](src/server.cpp): `server` application code.
- [src/client.cpp](src/client.cpp): `client` application code.

## Prerequisites
- `gcc`
- `git`
- `make`
- `docker` (optional)

### Getting Prerequisites on Debian
You can install `gcc`, `git` and `make` on a Debian based system with the following command on your terminal.
```bash
sudo apt install build-essential git make
```

As of the writing of this document, it is not recommended that you install Docker from the default PPAs provided by Debian. In order to install Docker, please follow the instructions listed on the [official website](https://docs.docker.com/engine/install/debian/#install-using-the-repository). After installing Docker you might still not be able to run the `docker` command. This is usually because your current user does not belong to the `docker` group. This can be easily fixed by running the following command and logging out of your session and logging back in (or just restarting your computer).

```bash
sudo usermod -aG docker <YOUR USERNAME>

# THIS WILL RESTART YOUR SYSTEM RIGHT AWAY!
# Be sure you saved all your progress beforehand
sudo reboot
```
**Note: Remeber to replace `<YOUR USERNAME>` with the name of your user account.**

## Cloning
Before running the code on this repository, you'll have to clone it into your system. You can do so by running the following command on your terminal.
```bash
git clone https://github.com/TheLastBilly/client-server-docker
```

This will create a directory called `client-server-docker` on your system, this will be the root of this repository. Simply `cd` into it and follow the next instructions.
```bash
cd client-server-docker
```

## Compiling
### With Docker
Assuming you do have Docker installed on your system, you can build this repository by simply running the [build.sh](./build.sh) script. You can do so by running the following commands from the root of this repository.

```bash
chmod +x ./build.sh
./build.sh
```

The script will create a Docker image on your system called `client-server-docker`, and will mount the [src/](src/) directory from your copy of this repository into a newly created, temporary container. It will also grab the UID and GID of your user so that you can access the files modified by the container. Lastly, it will use the [src/Makefile](src/Makefile) script to build `server` and `client` executables, both for x86 and aarch64.

The following files and directories will be created on your system:

#### For x86

- [src/build/x86/](src/build/): Build files made by the compilation process for x86.
- [src/build/x86/bin/](src/build/bin/): Contains the `server` and `client` executables for x86.
- [src/build/x86/obj/](src/build/obj/): Contains the object and dependency files created during the build process for x86.

#### For aarch64

- [src/build/aarch64/](src/build/): Build files made by the compilation process for aarch64.
- [src/build/aarch64/bin/](src/build/bin/): Contains the `server` and `client` executables for aarch64.
- [src/build/aarch64/obj/](src/build/obj/): Contains the object and dependency files created during the build process for aarch64.

You might have also noticed the following output in your terminal after running [build.sh](./build.sh):
```bash
# ...
build/x86/bin/server build/x86/bin/test.sock &
build/x86/bin/client build/x86/bin/test.sock VERSION
5b7b8dcc946580fb8321fceaa23bce2f58386931
build/x86/bin/client build/x86/bin/test.sock TEST
REJECTED
build/x86/bin/client build/x86/bin/test.sock QUIT
OK
```

This is the output of the `server` and `client` programs running on the docker container. They were executed by the `test` target defined on [src/Makefile](src/Makefile) (line 41).

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

This should run the `test` target defined on [src/Makefile](src/Makefile) (line 41) and generate the following output on your terminal:
```bash
# ...
make: Entering directory '/home/drevil/Documents/client-server-docker/src'
build/bin/server build/bin/test.sock &
build/bin/client build/bin/test.sock VERSION
5b7b8dcc946580fb8321fceaa23bce2f58386931
build/bin/client build/bin/test.sock TEST
REJECTED
build/bin/client build/bin/test.sock QUIT
OK
make: Leaving directory '/home/drevil/Documents/client-server-docker/src'
```
