#ifndef _MAZE_H_HEADER_
#define _MAZE_H_HEADER_

#include <vector>
#include <random>

#include "common.h"

// Represents a randomly generated maze of arbitrary dimensions (determined at initialization)
class Maze
{
    public:
        // Base unit
        struct Unit
        {
            Unit(int display_char, int color);
            const int display_char;
            const int color;
        };

        // Stores position of itself. Maze ref is stored to make sure our moves are valid!
        struct MoveableUnit : Unit
        {
            MoveableUnit(const Maze&, const Unit&, int a, int b);
            void move(int x, int y);
            Point position;
            const Maze& maze;
        };

        Maze(unsigned width, unsigned height, const Unit& floor, const Unit& wall, const Unit& player);
        const Unit& cell(unsigned x, unsigned y) const;
        bool cell_fogged(unsigned x, unsigned y) const;
        std::vector<const MoveableUnit*> moveable_units() const;
        void move_player(int x, int y);
        MoveableUnit& player();

        const unsigned width;
        const unsigned height;

        const Point entrance;
        const Point exit;

        const Unit floor;
        const Unit wall;

    private:
        void generate_maze();
        static void subdivide_grid(Maze& maze, std::mt19937& mersenne_twister, const Rect&);
        void uncover_fog(Point pos);
        unsigned cell_idx(unsigned x, unsigned y) const;

        MoveableUnit pl;
        std::vector<const Unit*> grid; // Pointer array representing contents of maze grid
        std::vector<bool> fog_grid; // Bool array representing which cells of the maze grid are uncovered by player movement
};

#endif