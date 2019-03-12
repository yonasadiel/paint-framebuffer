#include <thread>
#include <unistd.h>
#include <utility>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "controller/control.hpp"
#include "controller/renderer.hpp"
#include "drawable/drawable.hpp"
#include "drawable/polygon.hpp"
#include "framebuffer/framebuffer.hpp"

int main(int argc, char **args) {
    flogclear();
    FrameBuffer *framebuffer;
    try {
        framebuffer = new FrameBuffer();
    } catch (Exception e) {
        e.print();
        return 1;
    }

    bool run = true;
    std::vector<Drawable *> *objects = new std::vector<Drawable *>;

    Polygon* pesawat = new Polygon("images/pesawat.point", CRED, 1);
    pesawat->move(400, 400);
    pesawat->scale(5);
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
