#ifndef CONTROL
#define CONTROL

#include <vector>

#include "paint.hpp"
#include "control.hpp"

#define COMMAND_QUIT ' '
#define COMMAND_DRAW_RECTANGLE 'r'
#define COMMAND_DRAW_TRIANGLE 't'
#define COMMAND_CURSOR_MOVE_LEFT 'a'
#define COMMAND_CURSOR_MOVE_RIGHT 'd'
#define COMMAND_CURSOR_MOVE_UP 'w'
#define COMMAND_CURSOR_MOVE_DOWN 's'
#define COMMAND_ENTER '\n'

void readInput(Paint* paint);

#endif
