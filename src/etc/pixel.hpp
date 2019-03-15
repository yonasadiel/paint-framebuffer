#ifndef PIXEL
#define PIXEL

#include "color.hpp"

#define toColor(p) (p.alpha << 24 | p.red << 16 | p.green << 8 | p.blue)

struct pixel
{
    unsigned int red    = 0x0;
    unsigned int green  = 0x0;
    unsigned int blue   = 0x0;
    unsigned int alpha  = 0x00;
};

typedef struct pixel pixel;

#endif