#ifndef _CURSES_H_HEADER_
#define _CURSES_H_HEADER_

#include <unordered_map>
#include <functional>

#include "common.h"
#include "maze.h"

class CursesHandler
{
    public:
        CursesHandler();
        ~CursesHandler();
        void register_per_frame_callback(std::function<void(int)>);
        void register_handler(int key, std::function<void(int)>);
        void draw_char(int ch, int col, const Point&);
        void run_input_loop();

    private:
        int get_color(int color);
        int color_index;
        std::unordered_map<int, int> colors;
        std::unordered_map<int, std::function<void(int)>> handlers;
        std::vector<std::function<void(int)>> always_call_input;
};

#endif