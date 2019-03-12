#include <curses.h>
#include <vector>

#include "../framebuffer/framebuffer.hpp"
#include "../drawable/drawable.hpp"
#include "control.hpp"

void readInput(FrameBuffer *framebuffer, std::vector<Drawable *> *objects, bool *run) {
    initscr();
    timeout(-1);

    while (*run) {
        char c = getch();
        if (c == COMMAND_QUIT) {
            *run = false;
        }
    }
    endwin();
}
