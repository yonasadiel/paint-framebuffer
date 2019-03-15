#ifndef PNG
#define PNG

#include "pixel.hpp"

struct image
{
    pixel** pixels;
    int width; 
    int height;
};

typedef struct image image;

#endif