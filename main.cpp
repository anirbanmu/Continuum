#include <iostream>
#include <ncurses.h>

#include "curses.h"
#include "maze.h"

using namespace std;

int main(int argc, char** argv)
{
    auto curses = CursesHandler();
    auto maze = Maze(1024, 1024, Maze::Unit(' ', COLOR_BLACK), Maze::Unit('#', COLOR_RED));
    curses.render(maze, tuple<unsigned, unsigned>(0, 0));
    getch();
    return 0;
}