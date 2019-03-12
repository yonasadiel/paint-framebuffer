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
    pesawat->move(400, 400);
    pesawat->scale(5);
    objects->push_back(pesawat);
    objects->push_back(new Line(framebuffer->getXRes() / 2, framebuffer->getYRes() / 2, 8 * framebuffer->getXRes() / 8, 0 * framebuffer->getYRes() / 8, 0x0000FF9F));
    objects->push_back(new Line(framebuffer->getXRes() / 2, framebuffer->getYRes() / 2, 5 * framebuffer->getXRes() / 8, 0 * framebuffer->getYRes() / 8, 0X001FDFBF));
    objects->push_back(new Line(framebuffer->getXRes() / 2, framebuffer->getYRes() / 2, 4 * framebuffer->getXRes() / 8, 0 * framebuffer->getYRes() / 8, 0X003FBFDF));
    objects->push_back(new Line(framebuffer->getXRes() / 2, framebuffer->getYRes() / 2, 3 * framebuffer->getXRes() / 8, 0 * framebuffer->getYRes() / 8, 0X005F9FFF));
    objects->push_back(new Line(framebuffer->getXRes() / 2, framebuffer->getYRes() / 2, 0 * framebuffer->getXRes() / 8, 0 * framebuffer->getYRes() / 8, 0X007F7FDF));
    objects->push_back(new Line(framebuffer->getXRes() / 2, framebuffer->getYRes() / 2, 0 * framebuffer->getXRes() / 8, 3 * framebuffer->getYRes() / 8, 0X009F5FBF));
    objects->push_back(new Line(framebuffer->getXRes() / 2, framebuffer->getYRes() / 2, 0 * framebuffer->getXRes() / 8, 4 * framebuffer->getYRes() / 8, 0X00BF3F9F));
    objects->push_back(new Line(framebuffer->getXRes() / 2, framebuffer->getYRes() / 2, 0 * framebuffer->getXRes() / 8, 5 * framebuffer->getYRes() / 8, 0X00DF1F7F));
    objects->push_back(new Line(framebuffer->getXRes() / 2, framebuffer->getYRes() / 2, 0 * framebuffer->getXRes() / 8, 8 * framebuffer->getYRes() / 8, 0X00FF005F));
    objects->push_back(new Line(framebuffer->getXRes() / 2, framebuffer->getYRes() / 2, 3 * framebuffer->getXRes() / 8, 8 * framebuffer->getYRes() / 8, 0X00DF1F3F));
    objects->push_back(new Line(framebuffer->getXRes() / 2, framebuffer->getYRes() / 2, 4 * framebuffer->getXRes() / 8, 8 * framebuffer->getYRes() / 8, 0X00BF3F1F));
    objects->push_back(new Line(framebuffer->getXRes() / 2, framebuffer->getYRes() / 2, 5 * framebuffer->getXRes() / 8, 8 * framebuffer->getYRes() / 8, 0X009F5F00));
    objects->push_back(new Line(framebuffer->getXRes() / 2, framebuffer->getYRes() / 2, 8 * framebuffer->getXRes() / 8, 8 * framebuffer->getYRes() / 8, 0X007F7F1F));
    objects->push_back(new Line(framebuffer->getXRes() / 2, framebuffer->getYRes() / 2, 8 * framebuffer->getXRes() / 8, 5 * framebuffer->getYRes() / 8, 0X005F9F3F));
    objects->push_back(new Line(framebuffer->getXRes() / 2, framebuffer->getYRes() / 2, 8 * framebuffer->getXRes() / 8, 4 * framebuffer->getYRes() / 8, 0X003FBF5F));
    objects->push_back(new Line(framebuffer->getXRes() / 2, framebuffer->getYRes() / 2, 8 * framebuffer->getXRes() / 8, 3 * framebuffer->getYRes() / 8, 0X001FDF7F));

    std::thread *t0 = new std::thread(readInput, framebuffer, objects, &run);
    std::thread *t1 = new std::thread(draw, framebuffer, objects, &run);

    t0->join();
    t1->join();

    delete pesawat;
    delete objects;
    delete framebuffer;

    return 0;
}
