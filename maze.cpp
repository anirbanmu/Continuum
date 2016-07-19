#include <tuple>
#include <random>
#include <iostream>

#include "maze.h"

using namespace std;

Maze::Unit::Unit(int d, int c) : display_char(d), color(c)
{
}

Maze::Maze(unsigned w, unsigned h, const Unit& f, const Unit& wa) : width(w), height(h), floor(f), wall(wa), grid(width * height, &floor)
{
    generate_maze();
}

Maze::Unit*& Maze::cell(unsigned x, unsigned y)
{
    return grid[x + y * width];
}

Maze::Unit* const & Maze::cell(unsigned x, unsigned y) const
{
    return grid[x + y * width];
}

tuple<unsigned, unsigned> dimensions(const tuple<unsigned, unsigned>& rect_start, const tuple<unsigned, unsigned>& rect_end)
{
    return tuple<unsigned, unsigned>(get<0>(rect_end) - get<0>(rect_start), get<1>(rect_end) - get<1>(rect_start));
}

void Maze::subdivide_grid(Maze& maze, mt19937& mersenne_twister, tuple<unsigned, unsigned> rect_start, tuple<unsigned, unsigned> rect_end)
{
    const auto rect_size = dimensions(rect_start, rect_end);
    if (get<0>(rect_size) < 6 && get<1>(rect_size) < 6)
    {
        return;
    }

    // Choose between horizontal & vertical division randomly if the rectangle is a square. Otherwise, choose to divide the longer dimension.
    const bool horizontal = ((get<0>(rect_size) == get<1>(rect_size)) && mersenne_twister() % 2 == 0) || (get<1>(rect_size) > get<0>(rect_size));
    if (horizontal)
    {
        unsigned division_y = 1;
        while(division_y % 2 == 1)
        {
            division_y = get<1>(rect_start) + max(min(mersenne_twister() % get<1>(rect_size), static_cast<unsigned long>(0.70 * get<1>(rect_size))), static_cast<unsigned long>(0.30 * get<1>(rect_size)));
        }

        unsigned opening = 0;
        while (opening % 2 == 0)
        {
            opening = get<0>(rect_start) + mersenne_twister() % get<0>(rect_size);
        }

        for (unsigned x = get<0>(rect_start); x < get<0>(rect_end); ++x)
        {
            maze.cell(x, division_y) = opening == x ? &maze.floor : &maze.wall;
        }
        subdivide_grid(maze, mersenne_twister, rect_start, tuple<unsigned, unsigned>(get<0>(rect_end), division_y));
        subdivide_grid(maze, mersenne_twister, tuple<unsigned, unsigned>(get<0>(rect_start), division_y + 1), rect_end);
        return;
    }

    unsigned division_x = 1;
    while(division_x % 2 == 1)
    {
        division_x = get<0>(rect_start) + max(min(mersenne_twister() % get<0>(rect_size), static_cast<unsigned long>(0.70 * get<0>(rect_size))), static_cast<unsigned long>(0.30 * get<0>(rect_size)));
    }

    unsigned opening = 0;
    while (opening % 2 == 0)
    {
        opening = get<1>(rect_start) + mersenne_twister() % get<1>(rect_size);
    }
    for (unsigned y = get<1>(rect_start); y < get<1>(rect_end); ++y)
    {
        maze.cell(division_x, y) = opening == y ? &maze.floor : &maze.wall;
    }
    subdivide_grid(maze, mersenne_twister, rect_start, tuple<unsigned, unsigned>(division_x, get<1>(rect_end)));
    subdivide_grid(maze, mersenne_twister, tuple<unsigned, unsigned>(division_x + 1, get<1>(rect_start)), rect_end);
}

void Maze::generate_maze()
{
    random_device rd;
    mt19937 mersenne_twister(rd());
    subdivide_grid(*this, mersenne_twister, tuple<unsigned, unsigned>(0, 0), tuple<unsigned, unsigned>(width, height));
}