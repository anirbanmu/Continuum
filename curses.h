#ifndef _CURSES_H_HEADER_
#define _CURSES_H_HEADER_

#include <tuple>
#include <unordered_map>

#include "maze.h"

class CursesHandler
{
    public:
        CursesHandler();
        ~CursesHandler();
        void render(const Maze&, const std::tuple<unsigned, unsigned>&);

    private:
        int get_color(int color);
        int color_index;
        std::unordered_map<int, int> colors;
};

#endif