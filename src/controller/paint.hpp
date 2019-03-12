#ifndef PAINT
#define PAINT

#include <vector>

#include "../drawable/drawable.hpp"

class Paint {
private:
    std::vector<Drawable*>* layers;

public:
    Paint() {
        this->layers = new std::vector<Drawable*>();
    }
};

#endif
