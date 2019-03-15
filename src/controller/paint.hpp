#ifndef PAINT
#define PAINT

#include <queue>
#include <vector>

#include "../drawable/composite.hpp"
#include "../drawable/drawable.hpp"
#include "../drawable/polygon.hpp"
#include "../etc/color.hpp"
#include "../framebuffer/framebuffer.hpp"
#include "../framebuffer/modelbuffer.hpp"

#define STATE_IDLE 0
#define STATE_DRAWING_RECTANGLE_FIRST 1
#define STATE_DRAWING_RECTANGLE_SECOND 2
#define STATE_DRAWING_TRIANGLE_FIRST 3
#define STATE_DRAWING_TRIANGLE_SECOND 4
#define STATE_DRAWING_TRIANGLE_THIRD 5
#define STATE_DRAWING_LINE_FIRST 6
#define STATE_DRAWING_LINE_SECOND 7
#define STATE_SELECTING_OBJECT 8
#define STATE_OBJECT_SELECTED 9

#define ROTATION_SPEED 0.1
#define SCALING_SPEED 2

class Paint {
private:
    std::vector<Drawable*>* layers;
    Composite* cursor;
    bool cursorVisibility;
    bool running;
    unsigned int state;
    unsigned int nextState;
    Polygon* workingPolygon;
    color currentColor;
    int currentPattern;
    int width, height;

public:
    Paint() {
        this->layers = new std::vector<Drawable*>();
        this->running = true;
        this->cursorVisibility = false;
        this->cursor = new Composite("images/cursor.composite", CGRAY);
        this->cursor->scale(0.5);
        this->currentColor = CWHITE;
        this->currentPattern = 0;
        this->state = STATE_IDLE;
        this->nextState = STATE_IDLE;
    }

    Paint(int canvasWidth, int canvasHeight) : Paint() {
        this->width = canvasWidth;
        this->height = canvasHeight;
    }

    bool stillRunning() { return this->running; }
    void terminate() { this->running = false; }

    void showCursor() { this->cursorVisibility = true; }
    void hideCursor() { this->cursorVisibility = false; }

    void setFillColor(color fillColor) {
        this->currentColor = fillColor;
        if (this->workingPolygon != NULL) {
            this->workingPolygon->setFillColor(fillColor);
        }
    }

    void setPattern(int pattern) {
        this->currentPattern = pattern;
        if (this->workingPolygon != NULL) {
            this->workingPolygon->setPattern(pattern);
        }
    }

    void draw(IFrameBuffer* framebuffer, bool drawById = false) {
        color temp;
        for (int i = 0; i < layers->size(); i++) {
            Polygon* layer = (Polygon*)layers->at(i);
            if (drawById) {
                temp = (layer)->getFillColor();
                (layer)->setFillColor(i+1);
            }
            layer->draw(framebuffer);
            if (drawById) {
                (layer)->setFillColor(temp);
            }
        }
        if (this->workingPolygon != NULL)
            this->workingPolygon->draw(framebuffer);
        if (this->cursorVisibility)
            this->cursor->draw(framebuffer);
    }

    void pushWorkingPolygon() {
        this->layers->push_back(this->workingPolygon);
    }

    void deleteWorkingPolygon() {
        delete this->workingPolygon;
        this->workingPolygon = NULL;
        this->hideCursor();
        this->nextState = STATE_IDLE;
    }

    /*** STATE-related method ***/

    bool isIdle() { return this->state == STATE_IDLE; }
    bool isObjectSelected() {return this->state == STATE_OBJECT_SELECTED; }
    bool isAbleToMoveCursor() { return this->cursorVisibility; }
    void startDrawRectangle() { this->nextState = STATE_DRAWING_RECTANGLE_FIRST; }
    void startDrawTriangle() { this->nextState = STATE_DRAWING_TRIANGLE_FIRST; }
    void startDrawLine() { this->nextState = STATE_DRAWING_LINE_FIRST; }
    void startSelection() { this->nextState = STATE_SELECTING_OBJECT; }
    void rotateRight() { this->workingPolygon->rotate(ROTATION_SPEED); }
    void rotateLeft() { this->workingPolygon->rotate(-ROTATION_SPEED); }
    void scaleUp() { this->workingPolygon->scale(SCALING_SPEED); }
    void scaleDown() { this->workingPolygon->scale(1.0/SCALING_SPEED); }
    void moveCursor(int dx, int dy) {
        this->cursor->move(dx, dy);
        int cursorX = this->cursor->getAnchor()->getX();
        int cursorY = this->cursor->getAnchor()->getY();
        if (this->state == STATE_DRAWING_RECTANGLE_SECOND) {
            this->workingPolygon->pointAt(1)->setX(cursorX);
            this->workingPolygon->pointAt(2)->setX(cursorX);
            this->workingPolygon->pointAt(2)->setY(cursorY);
            this->workingPolygon->pointAt(3)->setY(cursorY);
        } else if (this->state == STATE_DRAWING_TRIANGLE_SECOND 
            || this->state == STATE_DRAWING_LINE_SECOND) {
            this->workingPolygon->pointAt(1)->setX(cursorX);
            this->workingPolygon->pointAt(1)->setY(cursorY);
        } else if (this->state == STATE_DRAWING_TRIANGLE_THIRD) {
            this->workingPolygon->pointAt(2)->setX(cursorX);
            this->workingPolygon->pointAt(2)->setY(cursorY);
        } else if (this->state == STATE_OBJECT_SELECTED) {
            this->workingPolygon->move(dx, dy);
        }
    }
    void goToNextState() { this->state = this->nextState; }
    void handleClick() {
        int cursorX = this->cursor->getAnchor()->getX();
        int cursorY = this->cursor->getAnchor()->getY();

        if (this->state == STATE_DRAWING_RECTANGLE_FIRST) {
            this->nextState = STATE_DRAWING_RECTANGLE_SECOND;

            std::vector<Coordinate*>* points = new std::vector<Coordinate*>();
            points->push_back(new Coordinate(cursorX, cursorY));
            points->push_back(new Coordinate(cursorX, cursorY));
            points->push_back(new Coordinate(cursorX, cursorY));
            points->push_back(new Coordinate(cursorX, cursorY));

            this->workingPolygon = new Polygon(points, this->currentColor, 0);
            this->workingPolygon->setOutlineColor(CRED);
        } else if (this->state == STATE_DRAWING_RECTANGLE_SECOND) {
            this->workingPolygon->setAnchorOnCenter();
            this->pushWorkingPolygon();
            this->hideCursor();
            this->nextState = STATE_IDLE;
            this->workingPolygon = NULL;
        } else if (this->state == STATE_DRAWING_TRIANGLE_FIRST) {
            this->nextState = STATE_DRAWING_TRIANGLE_SECOND;

            std::vector<Coordinate*>* points = new std::vector<Coordinate*>();
            points->push_back(new Coordinate(cursorX, cursorY));
            points->push_back(new Coordinate(cursorX, cursorY));
            points->push_back(new Coordinate(cursorX, cursorY));

            this->workingPolygon = new Polygon(points, this->currentColor, 0);
            this->workingPolygon->setOutlineColor(CRED);
        } else if (this->state == STATE_DRAWING_TRIANGLE_SECOND) {
            this->nextState = STATE_DRAWING_TRIANGLE_THIRD;
        } else if (this->state == STATE_DRAWING_TRIANGLE_THIRD) {
            this->workingPolygon->setAnchorOnCenter();
            this->pushWorkingPolygon();
            this->hideCursor();
            this->nextState = STATE_IDLE;
            this->workingPolygon = NULL;
        } else if (this->state == STATE_DRAWING_LINE_FIRST) {
            this->nextState = STATE_DRAWING_LINE_SECOND;

            std::vector<Coordinate*>* points = new std::vector<Coordinate*>();
            points->push_back(new Coordinate(cursorX, cursorY));
            points->push_back(new Coordinate(cursorX, cursorY));
            this->workingPolygon = new Polygon(points, this->currentColor, 0);
        } else if (this->state == STATE_DRAWING_LINE_SECOND) {
            this->pushWorkingPolygon();
            this->hideCursor();
            this->nextState = STATE_IDLE;
            this->workingPolygon = NULL;
        } else if (this->state == STATE_SELECTING_OBJECT) {
            selectObjectAt(cursorX, cursorY);
        } else if (this->state == STATE_OBJECT_SELECTED) {
            this->pushWorkingPolygon();
            this->hideCursor();
            this->nextState = STATE_IDLE;
            this->workingPolygon = NULL;
        }
    }

    void handlePushAtBack(){
        if (this->state == STATE_DRAWING_RECTANGLE_SECOND) {
            this->workingPolygon->setAnchorOnCenter();
            this->pushWorkingPolygonAtBackLayer();
            this->hideCursor();
            this->nextState = STATE_IDLE;
            this->workingPolygon = NULL;
        } else if (this->state == STATE_DRAWING_TRIANGLE_THIRD) {
            this->workingPolygon->setAnchorOnCenter();
            this->pushWorkingPolygonAtBackLayer();
            this->hideCursor();
            this->nextState = STATE_IDLE;
            this->workingPolygon = NULL;
        } else if (this->state == STATE_DRAWING_LINE_SECOND) {
            this->pushWorkingPolygonAtBackLayer();
            this->hideCursor();
            this->nextState = STATE_IDLE;
            this->workingPolygon = NULL;
        } else if (this->state == STATE_OBJECT_SELECTED) {
            this->pushWorkingPolygonAtBackLayer();
            this->hideCursor();
            this->nextState = STATE_IDLE;
            this->workingPolygon = NULL;
        }
    }

    void selectObjectAt(int x, int y) {
        ModelBuffer* selectionBuffer = new ModelBuffer(this->width, this->height, 0, 0);
        selectionBuffer->clearScreen();
        this->draw(selectionBuffer, true);
        int id = ((int)selectionBuffer->lazyCheck(new Coordinate(x, y)))-1;
        if (id >= 0) {
            this->workingPolygon = (Polygon *)this->layers->at(id);
            this->layers->erase(this->layers->begin()+id);
            this->nextState = STATE_OBJECT_SELECTED;
        } else {
            this->nextState = STATE_IDLE;
            this->hideCursor();
        }
    }

    void moveSelected(int x, int y){
        workingPolygon->move(x, y);
    }

    void pushWorkingPolygonAtBackLayer(){
        this->layers->insert(this->layers->begin(), this->workingPolygon);
    }

    void panScreen(int x, int y){
        for (int i = 0; i < layers->size(); i++){
            this->workingPolygon = (Polygon *)this->layers->at(i);
            moveSelected(x, y);
        }
    }
};

#endif
