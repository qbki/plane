# Plane Game Engine

## About

Plane Game Engine or just plane is a game engine
that's main goal is to create a constructor of
top-down shooters.

High level pipeline:

* Download executables or compile Plane Game Engine
* Prepare assets (3D models, sounds, translations)
* Describe levels, behavior, lights, particles,
  etc. in json files
* Run the game


## Documentation

The main source of information is [Plane Game Engine
Documentation](https://qbki.github.io/planedoc/).

All the tools in Plane Game Engine ecosystem are centered around [Plane Game
Engine Data Structures](https://qbki.github.io/planeds/).


## From the author

> What I cannot create I do not understand
  *- Richard Phillips Feynman*

It's my first attempt to create a game engine, so it's not a good idea to learn
from this project how to implement a game engine. I just learn how to create
things.


## Examples

Examples were moved into [a sepatare repository](https://github.com/qbki/plane-assets).
The main idea behind this decision is that you
fork this project and populate it with your assets
and code.


## Linux build

Run this command and install all system
dependencies that _conan_ will ask you:

```sh
$ make init && make build
```

In case of debian based distributives. System
dependencies are the dependencies that require
using _apt-get_ command with super user
privileges.

An executable will be located in the
_./build/linux/bin/_ directory.


## Web build

Run this command and install all system
dependencies that _conan_ will ask you:

```sh
$ make init-build-pack-wasm
```
It installs dependencies, builds the project and creates
a zip archive that contains a ready to run application,
just add your assets.

Artifacts will be placed in the _./build/pack/_ directory.


## Using Docker

Copy the _.env.default_ file in the project's root
directory as _.env_.

```sh
$ cp .env.default .env
```

Setup your USER_ID and GROUP_ID in the _.env_
file. To get them you can use _id_ utility.

```sh
$ id -u
1000
$ id -g
1000
```

To build a zip archive with an executable file run
this command:

```sh
$ make docker-build-wasm
```
Take a cup of tea, this is a long process. It
builds a docker image based on ubuntu, compiles
_conan_ dependencies, compiles the project itself
and creates a zip archive. Artifacts will be
placed in the _./build/docker_build/pack/_
directory.


## License

Plane Game Engine is [MIT licensed](./LICENSE).
