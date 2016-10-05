# image

A BMP to DDS converter (and the other way around)

## Compilation

These instructions assume that the project has been
cloned into a directory named `image`.

### Compilation in the source directory

    cd image
    make all test

## Running the tests

This project comes along with full suite of unit tests
that can be executed with the make command. E.g.:

    cd image
    make test

## Running

The converter is executed simply by giving it the name
of the file to be converted. The file type is detected
automatically and if the file has no problems the converted
file will be stored as `out.<filetype>`. E.g.:

    cd image
    ./image foobar.bmp

The resulting file will be `out.dds`.
