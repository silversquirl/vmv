# vmv

vktec and mlugg's visualiser

## What?

This is an audio visualiser using FFTW3, OpenGL and some serious hacks,
courtesy of vktec. It was written for fun, as well as for vktec to learn
how on earth Fourier transforms work. mlugg kindly did all the graphics
code, which vktec has no idea about.

## Installation

This will probably break from one machine to the next, but hopefully a
simple `make` will build it on a POSIX-compatible machine. If not,
something's probably broken with the Makefile; feel free to open an issue.

Configuration is done in `config.h` currently. You'll probably need to
tweak that for it to work properly.

## License

This software is licensed under the MIT license. See the `LICENSE`
file for more details on the license. Wow I typed "license" a lot there.
