# image

A BMP to DDS converter (and the other way around)

## Building

These instructions assume that the project has been cloned into a
directory named `image`. To build the project run the below commands.

    cd image
    make

This will create an executable named `image` in the project's root
directory.

## Testing

This project is written using Test Driven Development (TDD) practices.
To run the unit tests pass the `test` parameter to `make`.

    cd image
    make test

### Submodules

This project includes a Google Test unit testing framework as a git
submodule. The submodules are not cloned as part of the project by
default, in order to run the unit tests the below commands need to be
run once.

    cd image
    git submodule init
    git submodule update

This will initialize and update the submodules in the repository.
Another way to achieve this is to pass `--recursive` option when
cloning the project.

    git clone --recursive <repository>

## Running

The converter is executed simply by giving it the name
of the file to be converted. The file type is detected
automatically and if the file has no problems the converted
file will be stored as `out.<bmp|dds>`.

    cd image
    ./image foobar.bmp

In the above case the resulting file will be `out.dds`.
