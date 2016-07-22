CXX := clang++-3.8
CXXFLAGS := -O2 -Wall -Werror -std=c++14
LINKFLAGS := -lncurses

# Link
continuum: *.o
    $(CXX) $(CXXFLAGS) $(LINKFLAGS) $(inputs) -o $(output)

main.o: main.cpp
    $(CXX) $(CXXFLAGS) -c $(input) -o $(output)

curses.o: curses.cpp curses.h
    $(CXX) $(CXXFLAGS) -c curses.cpp -o $(output)

maze.o: maze.cpp maze.h
    $(CXX) $(CXXFLAGS) -c maze.cpp -o $(output)