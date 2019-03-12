#include <curses.h>
#include <vector>

#include "paint.hpp"
#include "control.hpp"

void readInput(Paint* paint) {
    initscr();
    timeout(-1);

    while (paint->stillRunning()) {
        char c = getch();
        if (c == COMMAND_QUIT) {
            paint->terminate();
        }
    }
    endwin();
}
