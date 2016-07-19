#include <tuple>
#include <random>
#include <iostream>

#include "maze.h"

using namespace std;

Maze::Unit::Unit(int d, int c) : display_char(d), color(c)
{
}

Maze::MoveableUnit::MoveableUnit(const Unit& u, int a, int b) : Unit(u), x(a), y(b)
{
}

Maze::Maze(unsigned w, unsigned h, const Unit& f, const Unit& wa, const Unit& pl) : width(w), height(h), floor(f), wall(wa), player(pl, 0, 0), grid(width * height, &floor)
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

unsigned long ensure_in_range(unsigned long value, unsigned long minimum, unsigned long maximum)
{
    return max(min(value, maximum), minimum);
}

void Maze::subdivide_grid(Maze& maze, mt19937& mersenne_twister, const Rect& rect)
{
    const auto rect_size = rect.dimensions();
    if (rect_size.x < 6 && rect_size.y < 6)
    {
        return;
    }

    // Choose between horizontal & vertical division randomly if the rectangle is a square. Otherwise, choose to divide the longer dimension.
    const bool horizontal = ((rect_size.x == rect_size.y) && mersenne_twister() % 2 == 0) || (rect_size.y > rect_size.x);
    if (horizontal)
    {
        unsigned division_y = 1;
        while(division_y % 2 == 1)
        {
            division_y = rect.start.y + ensure_in_range(mersenne_twister() % rect_size.y, 0.30 * rect_size.y, 0.70 * rect_size.y);
        }

        unsigned opening = 0;
        while (opening % 2 == 0)
        {
            opening = rect.start.x + mersenne_twister() % rect_size.x;
        }

        for (unsigned x = rect.start.x; x < rect.end.x; ++x)
        {
            maze.cell(x, division_y) = opening == x ? &maze.floor : &maze.wall;
        }
        subdivide_grid(maze, mersenne_twister, Rect(rect.start, Point(rect.end.x, division_y)));
        subdivide_grid(maze, mersenne_twister, Rect(Point(rect.start.x, division_y + 1), rect.end));
        return;
    }

    unsigned division_x = 1;
    while(division_x % 2 == 1)
    {
        division_x = rect.start.x + ensure_in_range(mersenne_twister() % rect_size.x, 0.30 * rect_size.x, 0.70 * rect_size.x);
    }

    unsigned opening = 0;
    while (opening % 2 == 0)
    {
        opening = rect.start.y + mersenne_twister() % rect_size.y;
    }
    for (unsigned y = rect.start.y; y < rect.end.y; ++y)
    {
        maze.cell(division_x, y) = opening == y ? &maze.floor : &maze.wall;
    }
    subdivide_grid(maze, mersenne_twister, Rect(rect.start, Point(division_x, rect.end.y)));
    subdivide_grid(maze, mersenne_twister, Rect(Point(division_x + 1, rect.start.y), rect.end));
}

void Maze::generate_maze()
{
    random_device rd;
    mt19937 mersenne_twister(rd());
    subdivide_grid(*this, mersenne_twister, Rect(0, 0, width, height));
}