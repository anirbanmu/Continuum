#include <algorithm>
#include <ncurses.h>
#include <iostream>
#include "curses.h"

using namespace std;

CursesHandler::CursesHandler() : color_index(1)
{
    initscr();
    start_color();
    use_default_colors();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
}

CursesHandler::~CursesHandler()
{
    endwin();
}

int CursesHandler::get_color(int color)
{
    auto search = colors.find(color);
    if (search != colors.end())
    {
        return search->second;
    }
    const auto index = color_index++;
    init_pair(index, color, -1);
    return colors[color] = index;
}

void CursesHandler::draw_char(int ch, int col, const Point& position, int attr)
{
    const unsigned color_index = get_color(col);
    attron(COLOR_PAIR(color_index) | attr);
    mvwaddch(stdscr, position.y, position.x, ch);
    attroff(COLOR_PAIR(color_index) | attr);
}

void CursesHandler::register_handler(int key, std::function<void(int)> func)
{
    handlers[key] = func;
}

void CursesHandler::register_per_frame_callback(std::function<void(int)> func)
{
    always_call_input.emplace_back(func);
}

void CursesHandler::run_input_loop()
{
    int ch = '0';
    while (ch != 'q')
    {
        ch = getch();

        // Do we need to call anyone for this key?
        auto search = handlers.find(ch);
        if (search != handlers.end())
        {
            search->second(ch);
        }

        for (auto call_always : always_call_input)
        {
            call_always(ch);
        }
    }
}