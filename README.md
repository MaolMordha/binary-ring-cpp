This project is an implementation of the [binary ring](http://www.complexification.net/gallery/machines/binaryRing/). You
can find a C implementation in [this repository](https://github.com/GalliumOS/xscreensaver), the source file being located
[here](https://github.com/GalliumOS/xscreensaver/blob/master/hacks/binaryring.c).

## Build instructions

This project uses [SFML](https://github.com/SFML/SFML) and [TCLAP](http://tclap.sourceforge.net/) for command line arguments parsing, so make sure both
are installed on your machine before building the project.

### build command
```bash
mkdir build && cd build && cmake .. && cmake --build .
```
