jamlib
======

JAM Library for GBA
by Josep Lluís Berral-García


Important note: you need the devkitpro deployed in /opt in order to compile GBA projects:
* /opt/devkitpro/devkitARM
* /opt/devkitpro/libgba

Also export the following environment variables:
* export DEVKITPRO=/opt/devkitpro
* export DEVKITARM=$DEVKITPRO/devkitARM

You can pick the example game and Makefile to start with something. When creating a new game, make sure that you:
* include <libjam.h> at the beginning of each file using libjam.
* include the libJAM header files (*.h) and library files (*.a) into the proper directories.