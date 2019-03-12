#ifndef PAINT
#define PAINT

#include <vector>

#include "../drawable/composite.hpp"
#include "../drawable/drawable.hpp"
#include "../drawable/polygon.hpp"
#include "../framebuffer/framebuffer.hpp"

class Paint {
private:
    std::vector<Drawable*>* layers;
    Composite* cursor;
    bool cursorVisibility;
    bool running;

public:
    Paint() {
        this->layers = new std::vector<Drawable*>();
        this->running = true;
        this->cursorVisibility = false;
        this->cursor = new Composite("images/cursor.composite", CWHITE);
        this->cursor->scale(0.5);
    }

    bool stillRunning() { return this->running; }
    void terminate() { this->running = false; }

    void showCursor() { this->cursorVisibility = true; }
    void hideCursor() { this->cursorVisibility = false; }

    void draw(FrameBuffer* framebuffer) {
        for (Drawable* layer : *layers) {
            layer->draw(framebuffer);
        }
        if (this->cursorVisibility) {
            this->cursor->draw(framebuffer);
        }
    }

    void moveCursor(int dx, int dy) {
        this->cursor->move(dx, dy);
    }
};

#endif
