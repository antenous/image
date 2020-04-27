# image

A BMP to DDS converter (and the other way around)

[![pipeline status](https://gitlab.com/antenous/image/badges/master/pipeline.svg)](https://gitlab.com/antenous/image/-/commits/master)
[![coverage report](https://gitlab.com/antenous/image/badges/master/coverage.svg)](https://gitlab.com/antenous/image/-/commits/master)

## Building

These instructions assume that the project has been cloned into a
directory named `image`. To build the project run the below commands.

```sh
$ cd image
$ cmake -H. -Bbuild/
$ cmake --build build/
```

This will create an executable named `image` in the `build/src` directory.

### Cross building for Windows

To build the project for Windows run the below commands.

```sh
$ cmake -H. -Bbuild/cross -DCMAKE_TOOLCHAIN_FILE=cmake/Toolchain-mingw-w64-x86_64.cmake
$ cmake --build build/cross
```

This will create an executable named `image.exe` in the `build/cross/src` directory.

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

Some IDEs (e.g. [Eclipse](https://www.eclipse.org/)) support building inside a
Docker container. To start the development container run the below command.

```sh
$ docker run \
  --rm -it -h image-docker \
  -v $PWD:$PWD -v $HOME/.ccache/:$HOME/.ccache/ \
  -w $PWD --name image-dev image:dev
```

Or to build the project within Docker from the outside run the below commands.

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

This project is written using Test Driven Development (TDD) practices. The
tests are written using [Google Test](https://github.com/google/googletest) C++
test framework which is downloaded and built together with the tests. To run
the unit tests build the project with
`check`.

```sh
$ cmake --build build/ --target check
```

This will build and run the unit tests and provide a summary of the results.
To get more detailed results run the `testrunner` in the `build/tst` directory.

```sh
$ ./build/tst/testrunner
```

**NOTE!** Unit tests are not available when cross building

### Code coverage

To create a code coverage report, configure the project with
`CODE_COVERAGE=On` and then build with `check-coverage`.

```sh
$ cmake -H. -Bbuild/ -DCODE_COVERAGE=On
$ cmake --build build/ --target check-coverage
```

This will produce the code coverage report in `build/image-coverage`
folder. View `build/image-coverage/index.html` to see the results.
If `gcovr` is installed the command above will show a summary of the
code coverage.

**NOTE!** Code coverage will enforce a `Debug` build

## Documentation

To create project documentation using `doxygen` build the project with
`doxygen-doc`.

```sh
$ cmake --build build/ --target doxygen-doc
```

This will create the documentation in `build/doxygen-doc` folder.
