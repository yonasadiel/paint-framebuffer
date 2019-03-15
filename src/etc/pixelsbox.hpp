#ifndef PIXELSBOX
#define PIXELSBOX

#include "pixel.hpp"

struct pixelsbox
{
    pixel** pixels;
    int width; 
    int height;
};

typedef struct pixelsbox pixelsbox;

#endif