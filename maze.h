#ifndef _MAZE_H_HEADER_
#define _MAZE_H_HEADER_

#include <vector>
#include <random>

#include "common.h"

// Represents a randomly generated maze of arbitrary dimensions (determined at initialization)
class Maze
{
    public:
        struct Unit
        {
            Unit(int display_char, int color);
            const int display_char;
            const int color;
        };

        struct MoveableUnit : Unit
        {
            MoveableUnit(const Unit&, int a, int b);
            Point position;
        };

        Maze(unsigned width, unsigned height, const Unit& floor, const Unit& wall, const Unit& player);
        Unit* const & cell(unsigned x, unsigned y) const;
        std::vector<const MoveableUnit*> moveable_units() const;

        const unsigned width;
        const unsigned height;

    private:
        Unit*& cell(unsigned x, unsigned y);
        void generate_maze();
        static void subdivide_grid(Maze& maze, std::mt19937& mersenne_twister, const Rect&);

        Unit floor;
        Unit wall;
        MoveableUnit player;
        std::vector<Unit*> grid; // Pointer array representing contents of maze grid
};

#endif