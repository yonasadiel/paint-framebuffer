#include <curses.h>
#include <vector>

#include "paint.hpp"
#include "control.hpp"

void readInput(Paint* paint) {
    initscr();
    timeout(-1);
    noecho();
    cbreak();

    int cursorSpeed = 5;
    int panSpeed = 5;
    unsigned char state = STATE_IDLE;

    while (paint->stillRunning()) {
        char c = getch();
        // we use nextState so it wont execute next interpretation before the state is changed
        unsigned char nextState = state;
        //printf("\n%d\n", 0+c);

        switch (c) {
            case CHOOSE_COLOR_1:
                paint->setFillColor(CWHITE);
                break;
            case CHOOSE_COLOR_2:
                paint->setFillColor(CRED);
                break;
            case CHOOSE_COLOR_3:
                paint->setFillColor(CGREEN);
                break;
            case CHOOSE_COLOR_4:
                paint->setFillColor(CBLUE);
                break;
            case CHOOSE_COLOR_5:
                paint->setFillColor(CYELLOW);
                break;
            case CHOOSE_COLOR_6:
                paint->setFillColor(CCYAN);
                break;
            case CHOOSE_COLOR_7:
                paint->setFillColor(CMAGENTA);
        }


        if (paint->isIdle()) {
            if (c == COMMAND_QUIT) {
                paint->terminate();
            } else if (c == COMMAND_DRAW_RECTANGLE) {
                paint->showCursor();
                paint->startDrawRectangle();
            } else if (c == COMMAND_DRAW_TRIANGLE) {
                paint->showCursor();
                paint->startDrawTriangle();
            } else if (c == COMMAND_DRAW_LINE) {
                paint->showCursor();
                paint->startDrawLine();
            } else if (c == COMMAND_SELECT) {
                paint->showCursor();
                paint->startSelection();
            } else if (c == COMMAND_PAN_LEFT) {
                paint->panScreen(-panSpeed, 0);
            } else if (c == COMMAND_PAN_RIGHT) {
                paint->panScreen(panSpeed, 0);
            } else if (c == COMMAND_PAN_UP) {
                paint->panScreen(0, -panSpeed);
            } else if (c == COMMAND_PAN_DOWN) {
                paint->panScreen(0, panSpeed);
            }
        }

        if (paint->isAbleToMoveCursor()) {
            if (c == COMMAND_CURSOR_MOVE_LEFT) {
                paint->moveCursor(-cursorSpeed, 0);
            } else if (c == COMMAND_CURSOR_MOVE_RIGHT) {
                paint->moveCursor(cursorSpeed, 0);
            } else if (c == COMMAND_CURSOR_MOVE_UP) {
                paint->moveCursor(0, -cursorSpeed);
            } else if (c == COMMAND_CURSOR_MOVE_DOWN) {
                paint->moveCursor(0, cursorSpeed);
            } else if (c == COMMAND_DELETE) {
                paint->deleteWorkingPolygon();
            }
        }

        if (c == COMMAND_ENTER) {
            paint->handleClick();
        }

        if (paint->isObjectSelected()) {
            if (c == COMMAND_ROTATE_RIGHT) {
                paint->rotateRight();
            } else if (c == COMMAND_ROTATE_LEFT) {
                paint->rotateLeft();
            } else if (c == COMMAND_SCALE_UP) {
                paint->scaleUp();
            } else if (c == COMMAND_SCALE_DOWN) {
                paint->scaleDown();
            }
        }

        paint->goToNextState();
    }
    endwin();
}
