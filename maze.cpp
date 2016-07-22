#include <tuple>
#include <random>
#include <iostream>

#include "maze.h"

using namespace std;

Maze::Unit::Unit(int d, int c) : display_char(d), color(c)
{
}

Maze::MoveableUnit::MoveableUnit(const Maze& m, const Unit& u, int x, int y) : Unit(u), position(x, y), maze(m)
{
}

void Maze::MoveableUnit::move(int x, int y)
{
    const auto new_position = Point(position.x + x, position.y + y);
    if (new_position.x < 0 || new_position.x + 1 > maze.width)
    {
        return;
    }
    if (new_position.y < 0 || new_position.y + 1 > maze.height)
    {
        return;
    }
    if (&maze.cell(new_position.x, new_position.y) != &maze.floor)
    {
        return;
    }
    position = new_position;
}

void Maze::move_player(int x, int y)
{
    player().move(x, y);
    uncover_fog(player().position);
}

void Maze::uncover_fog(Point pos)
{
    const unsigned uncover_size = 3;
    for (unsigned x = max<int>(0, pos.x - uncover_size); x <= min<int>(width - 1, pos.x + uncover_size); ++x)
    {
        for (unsigned y = max<int>(0, pos.y - uncover_size); y <= min<int>(height - 1, pos.y + uncover_size); ++y)
        {
            fog_grid[cell_idx(x, y)] = false;
        }
    }
}

Maze::Maze(unsigned w, unsigned h, const Unit& f, const Unit& wa, const Unit& p) : width(w), height(h), entrance(0, 0), exit(width - 1, height - 1), floor(f), wall(wa), pl(*this, p, 0, 0), grid(width * height, &floor), fog_grid(width * height, true)
{
    generate_maze();
    move_player(0, 0);
}

unsigned Maze::cell_idx(unsigned x, unsigned y) const
{
    return x + y * width;
}

const Maze::Unit& Maze::cell(unsigned x, unsigned y) const
{
    return *grid[cell_idx(x, y)];
}

bool Maze::cell_fogged(unsigned x, unsigned y) const
{
    return fog_grid[cell_idx(x, y)];
}

Maze::MoveableUnit& Maze::player()
{
    return pl;
}

vector<const Maze::MoveableUnit*> Maze::moveable_units() const
{
    return vector<const MoveableUnit*>(1, &pl);
}

unsigned long ensure_in_range(unsigned long value, unsigned long minimum, unsigned long maximum)
{
    return max(min(value, maximum), minimum);
}

// Recursively divide the given rect with one opening per division (so all parts of the rect are connected)
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
            maze.grid[x + division_y * maze.width] = opening == x ? &maze.floor : &maze.wall;
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
        maze.grid[division_x + y * maze.width] = opening == y ? &maze.floor : &maze.wall;
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