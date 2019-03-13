#include <thread>
#include <unistd.h>
#include <utility>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "controller/control.hpp"
#include "controller/paint.hpp"
#include "controller/renderer.hpp"
#include "drawable/drawable.hpp"
#include "drawable/polygon.hpp"
#include "framebuffer/framebuffer.hpp"

int main(int argc, char **args) {
    flogclear();
    FrameBuffer *framebuffer;
    try {
        framebuffer = new FrameBuffer();
    } catch (Exception* e) {
        e->print();
        return 1;
    }

    bool run = true;
    Paint* paint = new Paint();

    std::thread *t0 = new std::thread(readInput, paint);
    std::thread *t1 = new std::thread(draw, framebuffer, paint);

    t0->join();
    t1->join();

    delete framebuffer;

    return 0;
}
