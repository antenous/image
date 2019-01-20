# image

A BMP to DDS converter (and the other way around)

[![build status](https://gitlab.com/antenous/image/badges/master/build.svg)](https://gitlab.com/antenous/image/commits/master)
[![coverage report](https://gitlab.com/antenous/image/badges/master/coverage.svg)](https://gitlab.com/antenous/image/commits/master)

## Building

These instructions assume that the project has been cloned into a
directory named `image`. To build the project run the below commands.

```sh
$ cd image
$ cmake -H. -Bbuild/
$ cmake --build build/
```

This will create an executable named `image` in the `build/src` directory.

### Building with Docker

To separate the build process from the build system a [Docker](https://www.docker.com/)
development image is provided. Run the below commands to build the image

```sh
$ cmake --build build/ --target docker-dev-image
```

or

```sh
$ docker build \
    --build-arg USER=`id -un` \
    --build-arg UID=`id -u` \
    --build-arg GID=`id -g` \
    -t image:dev .
```

Some IDEs (e.g. Eclipse) support building inside a Docker container.
To build the project by hand using Docker run the below commands

```sh
$ docker run --rm -itd -v $PWD:$PWD -v ccache:/ccache --name image image:dev
$ docker exec -it image cmake -H$PWD -B$PWD/build/
$ docker exec -it image cmake --build $PWD/build/
$ docker stop image
```

**NOTE!** The Docker commands above may require root permissions


## Installation

By default, `install` target will install the executable in `/usr/local/bin/`.
To change the installation directory configure the project with `CMAKE_INSTALL_PREFIX`.

```sh
$ cmake -H. -Bbuild/ -DCMAKE_INSTALL_PREFIX=$PWD -DCMAKE_BUILD_TYPE=Release
$ cmake --build build/ --target install
```

The above command will install the executable in the `bin` directory.

**NOTE!** `install` target may require root permissions and is only available with `Release` builds

To uninstall build with `uninstall` or run `xargs rm < build/install_manifest.txt`.

## Running

To run the image converter pass the executable the name of the file to be
converted. The file type is detected automatically and if the file has no
problems the converted file will be stored as `out.<bmp|dds>` in the current
working dir.

```sh
$ bin/image foobar.bmp
```

In the above case the resulting file will be `out.dds`.

## Testing

This project is written using Test Driven Development (TDD) practices.
To run the unit tests build the project with `check`.

```sh
$ cmake --build build/ --target check
```

This will build and run the unit tests and provide a summary of the results.
To get more detailed results run the `testrunner` in the `build/tst` directory.

```sh
$ ./build/tst/testrunner
```

### Submodules

This project includes a Google Test unit testing framework as a git
submodule. The submodules are not cloned as part of the project by
default, in order to run the unit tests the below commands need to be
run once.

```sh
$ git submodule init
$ git submodule update
```

This will initialize and update the submodules in the repository.
Another way to achieve this is to pass `--recursive` option to `git`
when cloning this project.

Find more about git submodules from [here](https://git-scm.com/book/en/v2/Git-Tools-Submodules).

### Code coverage

To create a code coverage report, configure the project with
`ENABLE_CODE_COVERAGE=ON` and then build with `check-code-coverage`.

```sh
$ cmake -H. -Bbuild/ -DENABLE_CODE_COVERAGE=ON
$ cmake --build build/ --target check-code-coverage
```

This will produce the code coverage report in `build/image-coverage`
folder. View `build/image-coverage/index.html` to see the results.

To get a quick overview of the code coverage use `gcovr`.

```sh
$ gcovr -r . -e 3rdparty
```

**NOTE!** Code coverage will enforce a `Debug` build

## Documentation

To create project documentation using `doxygen` build the project with
`doxygen-doc`.

```sh
$ cmake --build build/ --target doxygen-doc
```

This will create the documentation in `build/doxygen-doc` folder.
