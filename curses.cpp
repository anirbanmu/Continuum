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

void CursesHandler::draw_unit(const Maze::Unit& u, const Point& position)
{
    const unsigned color_index = get_color(u.color);
    attron(COLOR_PAIR(color_index));
    mvwaddch(stdscr, position.y, position.x, u.display_char);
    attroff(COLOR_PAIR(color_index));
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
            draw_unit(*maze.cell(start.x + x, start.y + y), Point(start.x + x, start.y + y));
        }
    }

    for (auto u : maze.moveable_units())
    {
        draw_unit(*u, u->position);
    }

    wrefresh(stdscr);
}