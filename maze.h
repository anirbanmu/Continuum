#ifndef _MAZE_H_HEADER_
#define _MAZE_H_HEADER_

#include <vector>

// Represents a randomly generated maze of arbitrary dimensions (determined at initialization)
class Maze
{
    public:
        struct Unit
        {
            Unit(char display_char, int color);
            const char display_char;
            const int color;
        };

        Maze(unsigned width, unsigned height, const Unit& floor, const Unit& wall);
        Unit* const & cell(unsigned x, unsigned y) const;

        const unsigned width;
        const unsigned height;

    private:
        Unit*& cell(unsigned x, unsigned y);
        void generate_maze();

        Unit floor;
        Unit wall;
        std::vector<Unit*> grid; // Integer array representing contents of maze grid
};

#endif