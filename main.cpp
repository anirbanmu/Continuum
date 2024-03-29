#include <iostream>
#include <ncurses.h>
#include <cstring>
#include <type_traits>

#include "common.h"
#include "curses.h"
#include "maze.h"

using namespace std;

void draw_splash(CursesHandler& curses)
{
    const char* logo_text[] = { " _____             _   _                             ",
                                "/  __ \\           | | (_)                            ",
                                "| /  \\/ ___  _ __ | |_ _ _ __  _   _ _   _ _ __ ___  ",
                                "| |    / _ \\| '_ \\| __| | '_ \\| | | | | | | '_ ` _ \\ ",
                                "| \\__/\\ (_) | | | | |_| | | | | |_| | |_| | | | | | |",
                                " \\____/\\___/|_| |_|\\__|_|_| |_|\\__,_|\\__,_|_| |_| |_|",
                                "                                                     ",
                                "                                                     ",
                                "Press any key to start game or 'q' to exit. Use arroy keys to move player!"};

    int term_width = 0, term_height = 0;
    getmaxyx(stdscr, term_height, term_width);

    for (unsigned i = 0; i < extent<decltype(logo_text)>::value; ++i)
    {
        mvprintw(i + term_height / 2, (term_width - strlen(logo_text[i])) / 2, "%s", logo_text[i]);
    }

    wrefresh(stdscr);
}

void draw_frame(Maze& maze, CursesHandler& curses)
{
    int term_width = 0, term_height = 0;
    getmaxyx(stdscr, term_height, term_width);

    clear();

    const Point player_position = maze.player().position;

    if (player_position.x == maze.exit.x && player_position.y == maze.exit.y)
    {
        const char* win_message = "You've found the exit of the maze! Press 'q' to exit or continue exploring the maze!";
        mvprintw(term_height / 2, (term_width - strlen(win_message)) / 2, "%s", win_message);
        wrefresh(stdscr);
        return;
    }

    const auto start = Point(term_width * (player_position.x / term_width), term_height * (player_position.y / term_height));

    // Enables displaying greater than terminal dimension mazes!
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
    // Ignore negatives
    int maze_width = argc >= 2 ? atoi(argv[1]) : 2048;
    maze_width = maze_width < 0 ? 2048 : maze_width;

    int maze_height = argc >= 3 ? atoi(argv[2]) : 2048;
    maze_height = maze_height < 0 ? 2048 : maze_height;

    auto maze = Maze(maze_width, maze_height, Maze::Unit(' ', COLOR_BLACK), Maze::Unit('#', COLOR_GREEN), Maze::Unit('@', COLOR_WHITE));
    auto curses = CursesHandler();

    // Always draw our game upon input
    curses.register_per_frame_callback([&](int){ draw_frame(maze, curses); });

    // Call appropriate player movement functions on arrow key input
    curses.register_handler(KEY_DOWN, [&](int){ maze.move_player(0, 1); });
    curses.register_handler(KEY_UP, [&](int){ maze.move_player(0, -1); });
    curses.register_handler(KEY_LEFT, [&](int){ maze.move_player(-1, 0); });
    curses.register_handler(KEY_RIGHT, [&](int){ maze.move_player(1, 0); });

    draw_splash(curses);

    // Hand control over to the curses input handler
    curses.run_input_loop();
    return 0;
}