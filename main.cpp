#include <iostream>
#include <ncurses.h>

#include "common.h"
#include "curses.h"
#include "maze.h"

using namespace std;

int main(int argc, char** argv)
{
    auto curses = CursesHandler();
    auto maze = Maze(2048, 2048, Maze::Unit(' ', COLOR_BLACK), Maze::Unit('#', COLOR_GREEN), Maze::Unit('@', COLOR_WHITE));
    curses.render(maze, Point(0, 0));
    getch();
    return 0;
}