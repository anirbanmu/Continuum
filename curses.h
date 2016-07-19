#ifndef _CURSES_H_HEADER_
#define _CURSES_H_HEADER_

#include <unordered_map>

#include "common.h"
#include "maze.h"

class CursesHandler
{
    public:
        CursesHandler();
        ~CursesHandler();
        void render(const Maze&, const Point&);

    private:
        void draw_unit(const Maze::Unit&, const Point&);
        int get_color(int color);
        int color_index;
        std::unordered_map<int, int> colors;
};

#endif