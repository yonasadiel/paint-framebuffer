#ifndef PNG_HANDLER
#define PNG_HANDLER

#include <curses.h>
#include <stdlib.h>

#include <png.h>
#include "../etc/pixelsbox.hpp"

pixelsbox readPNG(char *filename);
void writePNG(char *filename, pixelsbox pxl);

#endif
