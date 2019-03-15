#ifndef POLYGON
#define POLYGON

#include <algorithm>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

#include "../controller/object.hpp"
#include "../etc/coordinate.hpp"
#include "../etc/pattern.hpp"
#include "../framebuffer/modelbuffer.hpp"
#include "../framebuffer/framebuffer.hpp"
#include "drawable.hpp"
#include "line.hpp"

class Polygon : public Drawable {
  protected:
    std::vector<Coordinate *> *points;
    Coordinate *anchor;
    color fillColor;
    color outlineColor;
    int pattern; // src/etc/pattern.hpp
    int mode; // 0 = color, 1 = pattern
    double scaleFactor;
    double rotation; // rad
    char id;         // polygon identifier
    PatternFactory patternFactory;

    class BressenhamTuple {
        public:
            int xStart, xIt, xEnd;
            int yStart, yIt, yEnd;
            int xSigned;
            int D, xD, yD;

            BressenhamTuple(Coordinate* from, Coordinate* to) {
                int dx = to->getX() - from->getX();
                int dy = to->getY() - from->getY();

                if (from->getY() < to->getY()) {
                    this->xStart = from->getX(); this->xEnd = to->getX();
                    this->yStart = from->getY(); this->yEnd = to->getY();
                } else {
                    this->xStart = to->getX(); this->xEnd = from->getX();
                    this->yStart = to->getY(); this->yEnd = from->getY();
                }
                this->yD = abs(dy);
                this->xD = abs(dx);
                this->xSigned = this->xD == 0 ? 0 : (this->xStart < this->xEnd ? 1 : -1);
                this->D = -this->xD;

                this->xIt = this->xStart;
                this->yIt = this->yStart;
            }
    };

  public:
    Polygon() {
        this->points = new std::vector<Coordinate *>();
        this->fillColor = CWHITE;
        this->pattern = 0;
        this->outlineColor = CWHITE;
        this->anchor = new Coordinate(0, 0);
        this->scaleFactor = 1;
        this->rotation = 0;
        this->id = NULL_OBJ;
        this->mode = 0;
    }

    Polygon(std::string filename, color c, char id) : Polygon() {
        int x, y;
        std::ifstream f(filename);
        while (f >> x >> y) {
            this->addPoint(new Coordinate(x, y));
        }
        f.close();
        this->setAnchorOnCenter();
        this->fillColor = c;
        this->pattern = 0;
        this->outlineColor = c;
        this->id = id;
    }

    Polygon(std::vector<Coordinate*>* points, color c, char id) : Polygon() {
        delete this->points;
        this->points = points;
        this->setAnchorOnCenter();
        this->fillColor = c;
        this->outlineColor = c;
        this->id = id;
    }

    ~Polygon() {
        for (int i = 0; i < this->points->size(); i++) {
            delete this->points->at(i);
        }
        delete this->points;
        delete this->anchor;
    }

    void setAnchorOnCenter() {
        Coordinate* center = this->getCenter();
        delete this->anchor;
        this->anchor = center;
    }

    void moveWithoutAnchor(int dx, int dy) {
        for (int i = 0; i < this->points->size(); i++) {
            this->points->at(i)->move(dx, dy);
        }
    }

    void addPoint(Coordinate* coordinate) {
        this->points->push_back(coordinate);
    }

    Coordinate* pointAt(int i) { return this->points->at(i); }
    Coordinate* getAnchor() const { return this->anchor; }

    void setAnchor(int x, int y) {
        this->anchor->setX(x);
        this->anchor->setY(y);
    }

    color getFillColor() { return this->fillColor; }
    color getPattern() { return this->pattern; }
    color getOutlineColor() { return this->outlineColor; }
    void setFillColor(color fillColor){
        this->fillColor = fillColor;
        this->mode = 0;
    }
    void setPattern(color pattern){
        this->pattern = pattern;
        this->mode = 1;
    }
    int getMode() {
        return this->mode;
    }
    void setMode(int mode) {
        this->mode = mode;
    }
    void setOutlineColor(color outlineColor) { this->outlineColor = outlineColor; }

    void move(int dx, int dy) {
        for (int i = 0; i < this->points->size(); i++) {
            this->points->at(i)->move(dx, dy);
        }
        this->anchor->move(dx, dy);
    }

    void moveTo(int x, int y, int maxVelocity = -1) {
        int dx = x - this->anchor->getX();
        int dy = y - this->anchor->getY();
        int dist2 = dx * dx + dy * dy;
        if (maxVelocity >= 0 && dist2 > maxVelocity*maxVelocity && (dx != 0 || dy != 0)) {
            double factor = maxVelocity * 1.0 / sqrt(dist2);
            dx *= factor;
            dy *= factor;
        }
        this->move(dx, dy);
    }

    void scale(double scaleFactor) {
        this->scaleFactor *= scaleFactor;
    }

    void linearScale(double scaleFactor) {
        this->scaleFactor += scaleFactor;
    }

    void scaleTo(double scaleFactor, double maxVelocity = 0) {
        if (this->scaleFactor > scaleFactor) {
            this->scaleFactor = std::max(this->scaleFactor - maxVelocity, scaleFactor);
        } else {
            this->scaleFactor = std::min(this->scaleFactor + maxVelocity, scaleFactor);
        }
    }

    void rotate(double rotation) {
        this->rotation += rotation;
    }

    void rotateTo(double rotation, double maxVelocity = 0) {
        if (this->rotation > rotation) {
            this->rotation = std::max(this->rotation - maxVelocity, rotation);
        } else {
            this->rotation = std::min(this->rotation + maxVelocity, rotation);
        }
    }

    double getScaleFactor() const { return this->scaleFactor; }
    double getRotation() const { return this->rotation; }
    char getId() const { return this->id; }

    Coordinate* getTransformedPoint(int i) {
        return this->points->at(i)->transform(this->scaleFactor, this->rotation, this->anchor);
    }

    void dilate(Coordinate* coor, double scaleFactor){
        for (int i = 0; i < points->size(); i++) {
            points->at(i)->setX((points->at(i)->getX() - coor->getX())*scaleFactor + coor->getY());
            points->at(i)->setY((points->at(i)->getY() - coor->getY())*scaleFactor + coor->getY());
        }
        anchor->setX((anchor->getX() - coor->getX())*scaleFactor + coor->getX());
        anchor->setY((anchor->getY() - coor->getY())*scaleFactor + coor->getY());
    }

    std::pair<Coordinate *, Coordinate *> *getBoundingBox() {
        int minX, maxX, minY, maxY;
        Coordinate *point = this->getTransformedPoint(0);
        minX = maxX = point->getX();
        minY = maxY = point->getY();
        delete point;
        for (int i = 1; i < this->points->size(); i++) {
            point = this->getTransformedPoint(i);
            minX = std::min(minX, point->getX());
            maxX = std::max(maxX, point->getX());
            minY = std::min(minY, point->getY());
            maxY = std::max(maxY, point->getY());
            delete point;
        }
        return new std::pair<Coordinate *, Coordinate *>(new Coordinate(minX, minY), new Coordinate(maxX, maxY));
    }

    Coordinate* getCenter() {
        std::pair<Coordinate *, Coordinate *> *boundingBox = this->getBoundingBox();
        int x = (boundingBox->first->getX() + boundingBox->second->getX()) / 2;
        int y = (boundingBox->first->getY() + boundingBox->second->getY()) / 2;
        delete boundingBox->first;
        delete boundingBox->second;
        delete boundingBox;
        return new Coordinate(x, y);
    }

    bool isOverlapping(std::pair<Coordinate *, Coordinate *> *otherBoundingBox) {
        std::pair<Coordinate *, Coordinate *> *thisBoundingBox = this->getBoundingBox();
        bool overlapX, overlapY;

        overlapX = otherBoundingBox->second->getX() >= thisBoundingBox->first->getX() &&
                   thisBoundingBox->second->getX() >= otherBoundingBox->first->getX();

        overlapY = otherBoundingBox->second->getY() >= thisBoundingBox->first->getY() &&
                   thisBoundingBox->second->getY() >= otherBoundingBox->first->getY();

        delete thisBoundingBox->first;
        delete thisBoundingBox->second;
        delete thisBoundingBox;
        return overlapX && overlapY;
    }

    void draw(IFrameBuffer *framebuffer) {
        std::pair<Coordinate *, Coordinate *> *boundingBox = this->getBoundingBox();
        int width = boundingBox->second->getX() - boundingBox->first->getX() + 1;
        int height = boundingBox->second->getY() - boundingBox->first->getY() + 1;

        ModelBuffer *modelBuffer = new ModelBuffer(width, height, boundingBox->first->getX(), boundingBox->first->getY());
        modelBuffer->clearScreen();

        std::vector<BressenhamTuple*>* lines = new std::vector<BressenhamTuple*>();
        for (int i = 0; i < this->points->size(); i++) {
            Coordinate* curr = this->getTransformedPoint(i);
            Coordinate* next = this->getTransformedPoint((i + 1) % this->points->size());
            lines->push_back(new BressenhamTuple(curr, next));
            delete curr;
            delete next;
        }

        Coordinate* coordinate = new Coordinate(0, 0);
		for (int yIt = boundingBox->first->getY(); yIt <= boundingBox->second->getY(); yIt++) {
            bool fill = false;
            for (int xIt = boundingBox->first->getX(); xIt <= boundingBox->second->getX(); xIt++) {
                bool colored = false;
                for (int i = 0; i < lines->size(); i++) {
                    BressenhamTuple* tuple = lines->at(i);
                    if (tuple->yIt == yIt && tuple->xIt == xIt) {
                        do {
                            coordinate->setX(tuple->xIt);
                            coordinate->setY(tuple->yIt);
                            modelBuffer->lazyDraw(coordinate, this->outlineColor);
                            if (tuple->D <= 0 && tuple->xIt != tuple->xEnd) {
                                tuple->D += 2 * tuple->yD;
                                tuple->xIt += tuple->xSigned;
                            }
                        } while (tuple->D <= 0 && tuple->xIt != tuple->xEnd);
                        tuple->D -= 2 * tuple->xD;
                        colored = true;
                        fill = !fill;
                        if (tuple->yIt != tuple->yEnd) {
                            tuple->yIt++;
                        } else {
                            tuple->yIt = -1;
                        }
                    }
                }
                if (fill) {
                    coordinate->setX(xIt);
                    coordinate->setY(yIt);
                    if (mode == 0) {
                        if (!colored) modelBuffer->lazyDraw(coordinate, this->fillColor);
                    } else { //mode = 1
                        unsigned int pixel = patternFactory.pat[pattern]->pixels[xIt%16][yIt%16];

                        if (!colored) modelBuffer->lazyDraw(coordinate, pixel);
                    }
                }
            }
		}
        delete coordinate;

        for (BressenhamTuple* tuple : *lines) {
            delete tuple;
        }
        delete lines;

        modelBuffer->flush((FrameBuffer *) framebuffer);

        delete modelBuffer;
    }

    void drawLines(ModelBuffer *modelBuffer) {
        int nLines = this->points->size();

        for (int i = 0; i < nLines; i++) {
            Coordinate *c1 = this->getTransformedPoint(i);
            Coordinate *c2 = this->getTransformedPoint((i + 1) % nLines);
            Line *line = new Line(c1->getX(), c1->getY(), c2->getX(), c2->getY(), this->outlineColor);
            line->draw(modelBuffer);
            delete c1;
            delete c2;
            delete line;
        }
    }
};

#endif