#ifndef CONTROL
#define CONTROL

#include <vector>

#include "paint.hpp"
#include "control.hpp"

#define COMMAND_QUIT ' '
#define COMMAND_DRAW_RECTANGLE 'r'
#define COMMAND_DRAW_TRIANGLE 't'
#define COMMAND_DRAW_LINE 'y'
#define COMMAND_CURSOR_MOVE_LEFT 'a'
#define COMMAND_CURSOR_MOVE_RIGHT 'd'
#define COMMAND_CURSOR_MOVE_UP 'w'
#define COMMAND_CURSOR_MOVE_DOWN 's'
#define COMMAND_ENTER '\n'

#define CHOOSE_COLOR_1 '1'
#define CHOOSE_COLOR_2 '2'
#define CHOOSE_COLOR_3 '3'
#define CHOOSE_COLOR_4 '4'
#define CHOOSE_COLOR_5 '5'
#define CHOOSE_COLOR_6 '6'
#define CHOOSE_COLOR_7 '7'


void readInput(Paint* paint);

#endif
