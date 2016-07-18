#ifndef _CURSES_H_HEADER_
#define _CURSES_H_HEADER_

#include <tuple>

#include "maze.h"

class CursesHandler
{
    public:
        CursesHandler();
        ~CursesHandler();
        void render(const Maze&, const std::tuple<unsigned, unsigned>&);
};

#endif