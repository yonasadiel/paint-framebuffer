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
#define COMMAND_PAN_LEFT 'a'
#define COMMAND_PAN_RIGHT 'd'
#define COMMAND_PAN_UP 'w'
#define COMMAND_PAN_DOWN 's'
#define COMMAND_ENTER '\n'
#define COMMAND_SELECT '\t'
#define COMMAND_DELETE '\\'
#define COMMAND_ROTATE_RIGHT 'e'
#define COMMAND_ROTATE_LEFT 'q'
#define COMMAND_SCALE_UP '='
#define COMMAND_SCALE_DOWN '-'
#define COMMAND_ENTER_AT_BACK '|'
#define COMMAND_RECORD_ANIMATION '/'

#define CHOOSE_COLOR_1 '1'
#define CHOOSE_COLOR_2 '2'
#define CHOOSE_COLOR_3 '3'
#define CHOOSE_COLOR_4 '4'
#define CHOOSE_COLOR_5 '5'
#define CHOOSE_COLOR_6 '6'
#define CHOOSE_COLOR_7 '7'

#define CHOOSE_PATTERN_1 '!'
#define CHOOSE_PATTERN_2 '@'
#define CHOOSE_PATTERN_3 '#'
#define CHOOSE_PATTERN_4 '$'
#define CHOOSE_PATTERN_5 '%'
#define CHOOSE_PATTERN_6 '^'
#define CHOOSE_PATTERN_7 '&'

void readInput(Paint* paint);

#endif
