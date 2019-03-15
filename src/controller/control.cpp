#include <curses.h>
#include <vector>
#include <iostream>

#include "paint.hpp"
#include "control.hpp"
#include "pnghandler.hpp"
#include "../etc/pixelsbox.hpp"
#include "../etc/utils.hpp"

void exportPNG(pixelsbox pxl) {
    std::cout << "\bEnter filename: ";
    char* filename = readLine();
    std::cout << "\rSaving to file " << filename << "....\n\r";

    writePNG(filename, pxl);

    std::cout << "Successfully Saved!\n\r";
    std::cout << "Press any key to continue...";
    getchar();
}

Rasterized* loadPNG() {
    std::cout << "\bEnter filename: ";
    char* filename = readLine();
    std::cout << "\rLoading from file " << filename << "....\n\r";

    pixelsbox pxl = readPNG(filename);
    Rasterized* r = new Rasterized(pxl.pixels, pxl.width, pxl.height, 0, 0);
}

void readInput(Paint* paint) {
    initscr();
    timeout(-1);
    noecho();
    cbreak();

    int cursorSpeed = 5;
    int panSpeed = 5;
    unsigned char state = STATE_IDLE;

    while (paint->stillRunning()) {
        if (!paint->isTextMode()) {
            char c = getch();
            unsigned char nextState = state;

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
                case CHOOSE_PATTERN_1:
                    paint->setPattern(0);
                    break;
                case CHOOSE_PATTERN_2:
                    paint->setPattern(1);
                    break;
                case CHOOSE_PATTERN_3:
                    paint->setPattern(2);
                    break;
                case CHOOSE_PATTERN_4:
                    paint->setPattern(3);
                    break;
                case CHOOSE_PATTERN_5:
                    paint->setPattern(4);
                    break;
                case CHOOSE_PATTERN_6:
                    paint->setPattern(5);
                    break;
                case CHOOSE_PATTERN_7:
                    paint->setPattern(6);
                    break;
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
                } else if (c == COMMAND_RECORD_ANIMATION){
                    if (paint->isRecording()) {
                        paint->stopRecording();
                    } else {
                        paint->startRecording();
                    }
                }
            }

            paint->goToNextState();
        } else {
            clear();
            for (int i = 0; i < 200; i++) 
                std::cout << "\n";

            if (paint->isSaving()) {
                exportPNG(paint->getSnapshot());
            } else if (paint->isLoading()) {
                paint->pushDrawable(loadPNG());
            }

            paint->exitTextMode();
        }
    }
    endwin();
}
