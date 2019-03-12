#include <curses.h>
#include <vector>

#include "paint.hpp"
#include "control.hpp"

void readInput(Paint* paint) {
    initscr();
    timeout(-1);

    int cursorSpeed = 2;

    while (paint->stillRunning()) {
        char c = getch();
        if (c == COMMAND_QUIT) {
            paint->terminate();
        } else if (c == COMMAND_DRAW_RECTANGLE) {
            paint->showCursor();
        } else if (c == COMMAND_CURSOR_MOVE_LEFT) {
            paint->moveCursor(-cursorSpeed, 0);
        } else if (c == COMMAND_CURSOR_MOVE_RIGHT) {
            paint->moveCursor(cursorSpeed, 0);
        } else if (c == COMMAND_CURSOR_MOVE_UP) {
            paint->moveCursor(0, -cursorSpeed);
        } else if (c == COMMAND_CURSOR_MOVE_DOWN) {
            paint->moveCursor(0, cursorSpeed);
        }
    }
    endwin();
}
