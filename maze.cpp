#include "maze.h"

using namespace std;

Maze::Unit::Unit(char d, int c) : display_char(d), color(c)
{
}

Maze::Maze(unsigned w, unsigned h, const Unit& f, const Unit& wa) : width(w), height(h), floor(f), wall(wa), grid(width * height, &wall)
{
}

Maze::Unit*& Maze::cell(unsigned x, unsigned y)
{
    return grid[x + y * width];
}

Maze::Unit* const & Maze::cell(unsigned x, unsigned y) const
{
    return grid[x + y * width];
}

void Maze::generate_maze()
{
}