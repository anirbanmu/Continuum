#include <iostream>
#include <ncurses.h>

#include "common.h"
#include "curses.h"
#include "maze.h"

using namespace std;

void draw_frame(Maze& maze, CursesHandler& curses)
{
    int term_width = 0, term_height = 0;
    getmaxyx(stdscr, term_height, term_width);

    const Point player_position = maze.player().position;
    const auto start = Point(player_position.x > 4 ? player_position.x - 4 : 0, player_position.y > 4 ? player_position.y - 4 : 0);

    const unsigned width = min(unsigned(term_width), maze.width - start.x);
    const unsigned height = min(unsigned(term_height), maze.height - start.y);
    for (unsigned x = 0; x < width; ++x)
    {
        for (unsigned y = 0; y < height; ++y)
        {
            const auto& cell = maze.cell(start.x + x, start.y + y);
            curses.draw_char(cell.display_char, cell.color, Point(x, y));
        }
    }

    for (auto u : maze.moveable_units())
    {
        curses.draw_char(u->display_char, u->color, Point(u->position.x - start.x, u->position.y - start.y));
    }

    wrefresh(stdscr);
}

int main(int argc, char** argv)
{
    auto maze = Maze(2048, 2048, Maze::Unit(' ', COLOR_BLACK), Maze::Unit('#', COLOR_GREEN), Maze::Unit('@', COLOR_WHITE));
    auto curses = CursesHandler();

    // Always draw our game upon input
    curses.register_per_frame_callback([&](int){ draw_frame(maze, curses); });
    draw_frame(maze, curses);

    // Call appropriate player movement functions until arrow key input
    curses.register_handler(KEY_DOWN, [&](int){ maze.player().move(0, 1); });
    curses.register_handler(KEY_UP, [&](int){ maze.player().move(0, -1); });
    curses.register_handler(KEY_LEFT, [&](int){ maze.player().move(-1, 0); });
    curses.register_handler(KEY_RIGHT, [&](int){ maze.player().move(1, 0); });

    curses.run_input_loop();
    return 0;
}