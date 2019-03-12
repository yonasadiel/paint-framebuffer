#include <curses.h>
#include <thread>
#include <unistd.h>
#include <utility>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "framebuffer/framebuffer.hpp"
#include "drawable/drawable.hpp"
#include "drawable/polygon.hpp"

#define PI acos(-1)

#define COMMAND_QUIT ' '

void readInput(FrameBuffer *framebuffer, std::vector<Drawable *> *objects, bool *run)
{
    initscr();
    timeout(-1);

    while (*run)
    {
        char c = getch();
        if (c == COMMAND_QUIT)
        {
            *run = false;
        }
    }
    endwin();
}

void draw(FrameBuffer *framebuffer, std::vector<Drawable *> *objects, bool *run)
{
    while (*run)
    {
        framebuffer->clearScreen();
        for (int i = 0; i < objects->size(); i++)
        {
            objects->at(i)->draw(framebuffer);
        }
        framebuffer->draw();
        usleep(10000);
    }
}

int main(int argc, char **args)
{
    flogclear();
    FrameBuffer *framebuffer;
    try
    {
        framebuffer = new FrameBuffer();
    }
    catch (Exception e)
    {
        e.print();
        return 1;
    }

    bool run = true;
    std::vector<Drawable *> *objects = new std::vector<Drawable *>;

    Polygon* pesawat = new Polygon("images/pesawat.point", CRED, 1);
    objects->push_back(pesawat);

    std::thread *t0 = new std::thread(readInput, framebuffer, objects, &run);
    std::thread *t1 = new std::thread(draw, framebuffer, objects, &run);

    t0->join();
    t1->join();

    delete pesawat;
    delete objects;
    delete framebuffer;

    return 0;
}
