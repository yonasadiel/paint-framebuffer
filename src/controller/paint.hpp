#ifndef PAINT
#define PAINT

#include <vector>

#include "../drawable/drawable.hpp"

class Paint {
private:
    std::vector<Drawable*>* layers;
    bool running;

public:
    Paint() {
        this->layers = new std::vector<Drawable*>();
        this->running = true;
    }

    bool stillRunning() { return this->running; }
    void terminate() { this->running = false; }

    int layerNum() { return this->layers->size(); }
    Drawable* layerAt(int i) { return this->layers->at(i); }
};

#endif
