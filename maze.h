#ifndef _MAZE_H_HEADER_
#define _MAZE_H_HEADER_

#include <vector>
#include <random>
#include <tuple>

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

        Maze(unsigned width, unsigned height, const Unit& floor, const Unit& wall);
        Unit* const & cell(unsigned x, unsigned y) const;

        const unsigned width;
        const unsigned height;

    private:
        Unit*& cell(unsigned x, unsigned y);
        void generate_maze();

        struct Point
        {
            int x, y;
            Point(int a, int b) : x(a), y(b){}
        };

        struct Rect
        {
            Rect(int left, int top, int right, int bottom) : start(left, top), end(right, bottom){}
            Rect(const Point& a, const Point& b) : start(a), end(b){}
            Point dimensions() const { return Point(end.x - start.x, end.y - start.y); }
            Point start;
            Point end;
        };

        static void subdivide_grid(Maze& maze, std::mt19937& mersenne_twister, const Rect&);

        Unit floor;
        Unit wall;
        std::vector<Unit*> grid; // Pointer array representing contents of maze grid
};

#endif