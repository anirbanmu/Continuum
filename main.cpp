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
    const auto start = Point(term_width * (player_position.x / term_width), term_height * (player_position.y / term_height));

    const unsigned width = min(unsigned(term_width), maze.width - start.x);
    const unsigned height = min(unsigned(term_height), maze.height - start.y);
    for (unsigned x = 0; x < width; ++x)
    {
        for (unsigned y = 0; y < height; ++y)
        {
            const auto& cell = maze.cell(start.x + x, start.y + y);
            curses.draw_char(cell.display_char, cell.color, Point(x, y), maze.cell_fogged(start.x + x, start.y + y) ? A_INVIS : 0);
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
    int maze_width = argc >= 2 ? atoi(argv[1]) : 2048;
    maze_width = maze_width < 0 ? 2048 : maze_width;

    int maze_height = argc >= 3 ? atoi(argv[2]) : 2048;
    maze_height = maze_height < 0 ? 2048 : maze_height;

    auto maze = Maze(maze_width, maze_height, Maze::Unit(' ', COLOR_BLACK), Maze::Unit('#', COLOR_GREEN), Maze::Unit('@', COLOR_WHITE));
    auto curses = CursesHandler();

    // Always draw our game upon input
    curses.register_per_frame_callback([&](int){ draw_frame(maze, curses); });
    draw_frame(maze, curses);

    // Call appropriate player movement functions on arrow key input
    curses.register_handler(KEY_DOWN, [&](int){ maze.move_player(0, 1); });
    curses.register_handler(KEY_UP, [&](int){ maze.move_player(0, -1); });
    curses.register_handler(KEY_LEFT, [&](int){ maze.move_player(-1, 0); });
    curses.register_handler(KEY_RIGHT, [&](int){ maze.move_player(1, 0); });

    curses.run_input_loop();
    return 0;
}