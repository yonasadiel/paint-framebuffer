#ifndef COLOR
#define COLOR

typedef unsigned int color;

#define CBLACK 0x0
#define CGRAY 0x00888888
#define CWHITE 0x00FFFFFF
#define CRED 0x00FF0000
#define CBLUE 0x000000FF
#define CGREEN 0x0000FF00
#define CYELLOW 0x00FFFF00
#define CCYAN 0x0000FFFF
#define CMAGENTA 0x00FF00FF

color colorMid(color a, color b, double p);

#endif