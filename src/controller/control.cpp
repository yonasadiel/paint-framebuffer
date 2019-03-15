#include <curses.h>
#include <vector>
#include <iostream>
#include <png.h>

#include "paint.hpp"
#include "control.hpp"

char* readLine() {
    char* buffer = new char[256];
    for (int i = 0; i < 256; i++) {
        buffer[i] = 0;
    }

    int idx = 0;
    
    while (true) {
        char x = getchar();
        if (x == 127 && idx > 0) {
            std::cout << "\b \b";
            idx--;
            buffer[idx] = 0;
        } if (x == 13) {
            std::cout << "\n";
            break;
        } else if ((x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z') || x == '-' || x == '_') {
            buffer[idx] = x;
            idx++;
        }
        std::cout << x;
    }
    
    return buffer;
}

void readInput(Paint* paint) {
    initscr();
    timeout(-1);

    int cursorSpeed = 5;
    unsigned char state = STATE_IDLE;

    while (paint->stillRunning()) {
        if (!paint->isTextMode()) {
            char c = getch();
            // we use nextState so it wont execute next interpretation before the state is changed
            unsigned char nextState = state;
            // printf("\n%d\n", 0+c);

            switch (c) {
                case CHOOSE_COLOR_1:
                    paint->setColor(CWHITE);
                    break;
                case CHOOSE_COLOR_2:
                    paint->setColor(CRED);
                    break;
                case CHOOSE_COLOR_3:
                    paint->setColor(CGREEN);
                    break;
                case CHOOSE_COLOR_4:
                    paint->setColor(CBLUE);
                    break;
                case CHOOSE_COLOR_5:
                    paint->setColor(CYELLOW);
                    break;
                case CHOOSE_COLOR_6:
                    paint->setColor(CCYAN);
                    break;
                case CHOOSE_COLOR_7:
                    paint->setColor(CMAGENTA);
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
                } else if (c == COMMAND_SAVE) {
                    paint->startSaving();
                } else if (c == COMMAND_LOAD) {
                    paint->startLoading();
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
        } else {
            clear();
            for (int i = 0; i < 200; i++) { std::cout << "\n"; }

            if (paint->isSaving()) {
                std::cout << "\bEnter filename: ";
                char* filename = readLine();
                std::cout << "\rSaving to file " << filename << "....\n\r";
                std::cout << "Successfully Saved!\n\r";
            } else if (paint->isLoading()) {
                std::cout << "\bEnter filename: ";
                char* filename = readLine();
                std::cout << "\rLoading from file " << filename << "....\n\r";
                std::cout << "Successfully Loaded!\n\r";
            } else {
                std::cout << "\bTEST ";
            }

            std::cout << "Press any key to continue...";
            getchar();
            paint->exitTextMode();
        }
    }
    endwin();
}
