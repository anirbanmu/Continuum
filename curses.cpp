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

void CursesHandler::render(const Maze& maze, const Point& start)
{
    int term_width = 0, term_height = 0;
    getmaxyx(stdscr, term_height, term_width);

    const unsigned width = min(unsigned(term_width), maze.width - start.x);
    const unsigned height = min(unsigned(term_height), maze.height - start.y);
    for (unsigned x = 0; x < width; ++x)
    {
        for (unsigned y = 0; y < height; ++y)
        {
            const auto& cell = *maze.cell(start.x + x, start.y + y);
            const unsigned color_index = get_color(cell.color);
            attron(COLOR_PAIR(color_index));
            mvwaddch(stdscr, y, x, cell.display_char);
            attroff(COLOR_PAIR(color_index));
        }
    }

    wrefresh(stdscr);
}