#include "color.hpp"

color colorMid(color a, color b, double p) {
    color c = CBLACK;
    c |= (((int) (((a >> 24) & 0xFF) * (1 - p) + ((b >> 24) & 0xFF) * p)) & 0xFF) << 24;
    c |= (((int) (((a >> 16) & 0xFF) * (1 - p) + ((b >> 16) & 0xFF) * p)) & 0xFF) << 16;
    c |= (((int) (((a >>  8) & 0xFF) * (1 - p) + ((b >>  8) & 0xFF) * p)) & 0xFF) <<  8;
    c |= (((int) (((a >>  0) & 0xFF) * (1 - p) + ((b >>  0) & 0xFF) * p)) & 0xFF) <<  0;
    return c;
}
