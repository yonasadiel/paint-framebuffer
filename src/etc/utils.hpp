#ifndef UTILS
#define UTILS

#include <iostream>
#include "color.hpp"

void flogclear();
void flog(std::string s);
void flog(double f);
void flog(int x);
void flogcolor(color c);
int pythagoreanApprox(int a, int b);
int ColorRGB(int R, int G, int B);

#endif