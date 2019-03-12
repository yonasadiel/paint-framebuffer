#ifndef PAINT
#define PAINT

#include <queue>
#include <vector>

#include "../drawable/composite.hpp"
#include "../drawable/drawable.hpp"
#include "../drawable/polygon.hpp"
#include "../framebuffer/framebuffer.hpp"

#define STATE_IDLE 0
#define STATE_DRAWING_RECTANGLE_FIRST 1
#define STATE_DRAWING_RECTANGLE_SECOND 2

class Paint {
private:
    std::vector<Drawable*>* layers;
    std::queue<Coordinate*>* pointQueue;
    Composite* cursor;
    bool cursorVisibility;
    bool running;
    unsigned int state;
    unsigned int nextState;
    Polygon* workingPolygon;

public:
    Paint() {
        this->layers = new std::vector<Drawable*>();
        this->running = true;
        this->cursorVisibility = false;
        this->cursor = new Composite("images/cursor.composite", CWHITE);
        this->cursor->scale(0.5);

        this->pointQueue = new std::queue<Coordinate*>();
        this->state = STATE_IDLE;
        this->nextState = STATE_IDLE;
    }

    bool stillRunning() { return this->running; }
    void terminate() { this->running = false; }

    void showCursor() { this->cursorVisibility = true; }
    void hideCursor() { this->cursorVisibility = false; }

    void draw(FrameBuffer* framebuffer) {
        for (Drawable* layer : *layers) {
            layer->draw(framebuffer);
        }
        if (this->workingPolygon != NULL)
            this->workingPolygon->draw(framebuffer);
        if (this->cursorVisibility)
            this->cursor->draw(framebuffer);
    }

    void addPointToQueue() {
        this->pointQueue->push(new Coordinate(this->cursor->getAnchor()));
    }

    void createRectangle() {
        Coordinate* c1 = this->pointQueue->front();
        this->pointQueue->pop();
        Coordinate* c2 = this->pointQueue->front();
        this->pointQueue->pop();

        std::vector<Coordinate*>* points = new std::vector<Coordinate*>();
        points->push_back(new Coordinate(c1->getX(), c1->getY()));
        points->push_back(new Coordinate(c1->getX(), c2->getY()));
        points->push_back(new Coordinate(c2->getX(), c2->getY()));
        points->push_back(new Coordinate(c2->getX(), c1->getY()));

        this->layers->push_back(new Polygon(points, CWHITE, 0));

        delete c1;
        delete c2;
    }

    /*** STATE-related method ***/

    bool isIdle() { return this->state == STATE_IDLE; }
    bool isAbleToMoveCursor() { return this->cursorVisibility; }
    void startDrawRectangle() { this->nextState = STATE_DRAWING_RECTANGLE_FIRST; }
    void moveCursor(int dx, int dy) {
        this->cursor->move(dx, dy);
        int cursorX = this->cursor->getAnchor()->getX();
        int cursorY = this->cursor->getAnchor()->getY();
        if (this->state == STATE_DRAWING_RECTANGLE_SECOND) {
            this->workingPolygon->pointAt(1)->setX(cursorX);
            this->workingPolygon->pointAt(2)->setX(cursorX);
            this->workingPolygon->pointAt(2)->setY(cursorY);
            this->workingPolygon->pointAt(3)->setY(cursorY);
        }
    }
    void goToNextState() { this->state = this->nextState; }
    void handleClick() {
        if (this->state == STATE_DRAWING_RECTANGLE_FIRST) {
            this->addPointToQueue();
            this->nextState = STATE_DRAWING_RECTANGLE_SECOND;
            int cursorX = this->cursor->getAnchor()->getX();
            int cursorY = this->cursor->getAnchor()->getY();

            std::vector<Coordinate*>* points = new std::vector<Coordinate*>();
            points->push_back(new Coordinate(cursorX, cursorY));
            points->push_back(new Coordinate(cursorX, cursorY));
            points->push_back(new Coordinate(cursorX, cursorY));
            points->push_back(new Coordinate(cursorX, cursorY));

            this->workingPolygon = new Polygon(points, CGRAY, 0);
        } else if (this->state == STATE_DRAWING_RECTANGLE_SECOND) {
            this->addPointToQueue();
            this->createRectangle();
            this->hideCursor();
            this->nextState = STATE_IDLE;
            Polygon* temp = this->workingPolygon;
            this->workingPolygon = NULL;
            delete temp;
        }
    }

};

#endif
