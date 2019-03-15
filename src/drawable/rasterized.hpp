#ifndef RASTERIZED
#define RASTERIZED

#include <algorithm>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

#include "../controller/object.hpp"
#include "../etc/coordinate.hpp"
#include "../etc/pixel.hpp"
#include "../framebuffer/modelbuffer.hpp"
#include "../framebuffer/framebuffer.hpp"
#include "drawable.hpp"
#include "line.hpp"

class Rasterized : public Drawable {
  protected:
    pixel** pixels;
    Coordinate *anchor;
    int width;
    int height;
    int xOffset;
    int yOffset;

  public:
    Rasterized(pixel** pixels, int width, int height, int xOffset, int yOffset) {
        this->pixels = pixels;
        this->anchor = new Coordinate(width/2, height/2);
        this->width = width;
        this->height = height;
        this->xOffset = xOffset;
        this->yOffset = yOffset;
    }

    ~Rasterized() {
        for (int i = 0; i < this->height; i++) {
            delete this->pixels[i];
        }
        delete this->pixels;
        delete this->anchor;
    }

    Coordinate* getAnchor() const { return this->anchor; }

    void setAnchor(int x, int y) {
        this->anchor->setX(x);
        this->anchor->setY(y);
    }

    std::pair<Coordinate *, Coordinate *> *getBoundingBox() {
        return new std::pair<Coordinate *, Coordinate *>(
            new Coordinate(this->xOffset, this->yOffset), 
            new Coordinate(this->xOffset + this->width, this->yOffset + this->height)
        );
    }

    Coordinate* getCenter() {
        return new Coordinate(this->width/2, this->height/2);
    }
    
    void draw(IFrameBuffer *framebuffer) {
        ModelBuffer *modelBuffer = new ModelBuffer(this->width, this->height, this->xOffset, this->yOffset);
        modelBuffer->clearScreen();

        Coordinate* coordinate = new Coordinate(0, 0);
		for (int row = 0; row < this->height; row++) {
            for (int column = 0; column < this->width; column++) {
                coordinate->setX(column + this->xOffset);
                coordinate->setY(row + this->yOffset);
                modelBuffer->lazyDraw(coordinate, toColor(this->pixels[row][column]));
            }
		}

        modelBuffer->flush((FrameBuffer *) framebuffer);

        delete coordinate;
        delete modelBuffer;
    }
};

#endif