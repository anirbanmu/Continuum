#include <algorithm>
#include <ncurses.h>

#include "curses.h"

using namespace std;

CursesHandler::CursesHandler()
{
    initscr();
    start_color();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    use_default_colors();
}

CursesHandler::~CursesHandler()
{
    endwin();
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
            init_pair(1, cell.color, -1);
            attron(COLOR_PAIR(1));
            mvwaddch(stdscr, y_incr, x_incr, cell.display_char);
            attroff(COLOR_PAIR(1));
        }
    }

    wrefresh(stdscr);
}