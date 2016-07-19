#include <algorithm>
#include <ncurses.h>
#include <iostream>
#include "curses.h"

using namespace std;

CursesHandler::CursesHandler() : color_index(0)
{
    initscr();
    start_color();
    use_default_colors();
    raw();
    keypad(stdscr, TRUE);
    noecho();
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

void CursesHandler::render(const Maze& maze, const tuple<unsigned, unsigned>& start)
{
    int term_width = 0, term_height = 0;
    getmaxyx(stdscr, term_height, term_width);

    const unsigned width = min(unsigned(term_width), maze.width - get<0>(start));
    const unsigned height = min(unsigned(term_height), maze.height - get<1>(start));
    for (unsigned x_incr = 0; x_incr < width; ++x_incr)
    {
        for (unsigned y_incr = 0; y_incr < height; ++y_incr)
        {
            const auto& cell = *maze.cell(get<0>(start) + x_incr, get<1>(start) + y_incr);
            const unsigned color_index = get_color(cell.color);
            attron(COLOR_PAIR(color_index));
            mvwaddch(stdscr, y_incr, x_incr, cell.display_char);
            attroff(COLOR_PAIR(color_index));
        }
    }

    wrefresh(stdscr);
}