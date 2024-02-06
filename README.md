# Plane Engine

## About

Plane Engine or just plane is a game engine that
have a main goal to create a constructor of
top-down shooters.

High level pipeline:

* Download executables or compile the project;
* Prepare models in 3D editor (Blender for
  instance);
* Describe levels, behavior, lights, particles,
  etc. in json files;
* Run the game;
* Try to correct errors;
* Read documentation and the API specification;
* Correct errors;
* ...
* PROFIT!

Sources of inspiration are games like
"Crimsonland", "Alien Shooter", "Earth Defence
Force".

## From the author

It's my first attempt to create a game engine, so
it's not a good idea to learn from this project
how to implement a game engine architecture. I
just learn how to create things and it helps me to
collect requirements through prototyping.

The closest goal is to create a game that has
these constraints:

* A game map that has size of one screen, no
  borders, just a flat surface;
* AI sharpened for crowd control;
* No multiplayer;
* No touch devices support;

## Requirements

| Name | Required | Comment |
| ---- | -------- | ------- |
| conan | yes | |
| cmake | yes | |
| make | yes | |
| zip | yes | |
| gcc | no | if you want compile a Linux build |
| docker | no | if you want try this project and keep your system clean, it could be the only dependency |


## Linux build

Run this command and install all system
dependencies that _conan_ will ask you:

```sh
$ make init && make build
```

In case of debian based distributives. System
dependencies are dependencies that require using
_apt-get_ command with super user privileges.

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

Run this command:

```sh
$ make docker-build-wasm
```
Take a cup of tea, this is a long process. It
builds a docker image based on ubuntu, compiles
_conan_ dependencies, compiles the project itself
and creates a zip archive. Artifacts will be
placed in the _./build/docker_build/pack/_
directory.
