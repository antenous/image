# image

A BMP to DDS converter (and the other way around)

[![build status](https://gitlab.com/antenous/image/badges/master/build.svg)](https://gitlab.com/antenous/image/commits/master)
[![coverage report](https://gitlab.com/antenous/image/badges/master/coverage.svg)](https://gitlab.com/antenous/image/commits/master)

## Building

These instructions assume that the project has been cloned into a
directory named `image`. To build the project run the below commands.

```sh
$ cd image
$ mkdir build
$ cd build
$ cmake ../
$ make
```

This will create an executable named `image` in the `src` directory.

## Installation

By default, `make install` will install the executable in `/usr/local/bin/`.
To change the installation directory configure the project with `CMAKE_INSTALL_PREFIX`.

```sh
$ cmake -DCMAKE_INSTALL_PREFIX=$PWD -DCMAKE_BUILD_TYPE=Release ../
$ make install
```

The above command will install the executable in the `bin` directory.

**NOTE!** `make install` may require root permissions and is only available with `Release` builds

To uninstall run `make uninstall` or `xargs rm < install_manifest.txt`.

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
$ make check
```

This will build and run the unit tests and provide a summary of the results.
To get more detailed results run the `testrunner` in the `tst` directory.

```sh
$ tst/testrunner
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
$ cmake -DENABLE_CODE_COVERAGE=ON ../
$ make check-code-coverage
```

This will produce the code coverage report in `image-coverage/`
folder. View `image-coverage/index.html` to see the results.

To get a quick overview of the code coverage use `gcovr`.

```sh
$ gcovr -r ../ -e 3rdparty
```

**NOTE!** Code coverage will enforce a `Debug` build

## Documentation

To create project documentation using `doxygen` build the project with
`doxygen-doc`.

```sh
$ make doxygen-doc
```

This will create the documentation in `doxygen-doc` folder.
