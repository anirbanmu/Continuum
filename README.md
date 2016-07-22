# Continuum
A simple rogue-like game written in C++.

### Introduction
The maze is generated by recursively subdividing our full grid of arbitrary size. The start of the maze is always at the top left (0, 0) and the exit of the maze is at (m-1, n-1) where our maze of dimension m x n. Displaying the maze, player movement and handling input is done via the ncurses library.

### Build instructions
The following are required to make Continuum: makepp, ncurses, clang++ (or g++). On a Debian system one can issue the following apt-get command to install of these dependencies:

    apt-get install libncurses5 libncurses5-dev makepp clang-3.8 build-essential

After having the dependencies installed, run the command `makepp` from the directory you've put the project in. This should build object files and then a final runnable binary named `continuum`. If you want to use a different version of the clang++ compiler or g++, please replace the `CXX :=` line in Makefile to whatever binary is most appropriate for you.

### Commandline options
You can pass the dimensions of the maze you want generated as two integers in x, y order when running the binary. The maze generation uses 6 x 6 as the smallest base condition to stop subdividing rectangles, so for best results please use dimensions greater than 6 for both x and y!
