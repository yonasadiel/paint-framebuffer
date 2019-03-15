#ifndef PIXEL
#define PIXEL

#include "color.hpp"

#define toColor(p) (p.red << 16 | p.green << 8 | p.blue)

struct pixel
{
    char red     = 0;
    char green   = 0;
    char blue    = 0;
};

typedef struct pixel pixel;

#endif