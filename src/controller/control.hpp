#ifndef CONTROL
#define CONTROL

#include <vector>

#include "../framebuffer/framebuffer.hpp"
#include "../drawable/drawable.hpp"
#include "control.hpp"

#define COMMAND_QUIT ' '
#define COMMAND_DRAW_RECTANGLE 'r'

void readInput(FrameBuffer *framebuffer, std::vector<Drawable *> *objects, bool *run);

#endif
