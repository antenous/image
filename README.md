# image

A BMP to DDS converter (and the other way around)

## Building

These instructions assume that the project has been cloned into a
directory named `image`. To build the project run the below commands.

```sh
cd image
./bootstrap
./configure
make
```

This will create an executable named `image` in the `src` directory.

## Installation

By default, `make install` will install the executable in `/usr/local/bin/`.
To change the installation directory configure the project with `--prefix`.

```sh
./configure --prefix=$PWD
make install
```

The above command will install the executable in `bin` directory located
in the current directory.

**NOTE!** `make install` may require root permissions

## Running

To run the image converter pass the executable the name of the file to be
converted. The file type is detected automatically and if the file has no
problems the converted file will be stored as `out.<bmp|dds>`.

```sh
bin/image foobar.bmp
```

In the above case the resulting file will be `out.dds`.

## Testing

This project is written using Test Driven Development (TDD) practices.
To run the unit tests build the project with `check`.

```sh
make check
```

This will build and run the unit tests and provide a summary of the resuts.
To get more detailed results run the `testrunner` in the `tst` directory.

```sh
tst/testrunner
```

### Submodules

This project includes a Google Test unit testing framework as a git
submodule. The submodules are not cloned as part of the project by
default, in order to run the unit tests the below commands need to be
run once.

```sh
git submodule init
git submodule update
```

This will initialize and update the submodules in the repository.
Another way to achieve this is to pass `--recursive` option to `git`
when cloning this project.

### Code coverage

To create a code coverage report, configure the project with
`--enable-code-coverage` and then build with `check-code-coverage`.

```sh
./configure --enable-code-coverage
make check-code-coverage
```

This will produce the code coverage report in `image-<version>-coverage/`
folder. View `image-<version>-coverage/index.html` to see the results.

To get a quick overview of the code coverage use `gcovr`.

```sh
make check
gcovr -r .
```

## Documentation

The create project documentation using `doxygen` build the project with
`doxygen-doc`.

```sh
make doxygen-doc
```

This will create the documentation in `doxygen-doc` folder.
