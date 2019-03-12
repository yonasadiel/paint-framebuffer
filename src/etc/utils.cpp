#include <fstream>
#include <iostream>
#include <ctime>

#include "color.hpp"
#include "utils.hpp"

#define log_filename "log/info.log"

void flogclear()
{
    std::ofstream fout(log_filename, std::ios::out);
    std::time_t t = std::time(0);
    struct tm *tmp = gmtime(&t);

    int h = (t / 360) % 24;
    int m = (t / 60) % 60;
    int s = t % 60;

    std::tm *now = std::localtime(&t);
    fout << (now->tm_year + 1900) << '-'
         << (now->tm_mon + 1) << '-'
         << now->tm_mday << ' '
         << h << ':' << m << ':' << s
         << "\n";
    fout.close();
}

void flog(std::string s)
{
    std::ofstream fout(log_filename, std::ios::out | std::ios::app);
    fout << s;
    fout.close();
}

void flog(double f)
{
    std::ofstream fout(log_filename, std::ios::out | std::ios::app);
    fout << f;
    fout.close();
}

void flog(int x)
{
    std::ofstream fout(log_filename, std::ios::out | std::ios::app);
    fout << x;
    fout.close();
}

void flogcolor(color c)
{
    std::ofstream fout(log_filename, std::ios::out | std::ios::app);
    char outc = ' ';
    if (c == CBLACK)
        outc = 'B';
    else if (c == CBLUE)
        outc = 'L';
    else if (c == CCYAN)
        outc = 'C';
    else if (c == CGREEN)
        outc = 'G';
    else if (c == CRED)
        outc = 'R';
    else if (c == CYELLOW)
        outc = 'Y';
    fout << outc;
    fout.close();
}

int pythagoreanApprox(int a, int b)
{
    if (a > b)
    {
        return a + b * 3 / 7;
    }
    else
    {
        return b + a * 3 / 7;
    }
}

int ColorRGB(char R, char G, char B)
{
    return R << 16 + G << 8 + B;
}