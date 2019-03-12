#include <vector>

#include "../framebuffer/framebuffer.hpp"
#include "../drawable/drawable.hpp"
#include "renderer.hpp"

void draw(FrameBuffer *framebuffer, std::vector<Drawable *> *objects, bool *run) {
    while (*run) {
        framebuffer->clearScreen();
        for (int i = 0; i < objects->size(); i++) {
            objects->at(i)->draw(framebuffer);
        }
        framebuffer->draw();
        usleep(10000);
    }
}
