#ifndef POLYGON
#define POLYGON

#include <algorithm>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

#include "../etc/coordinate.hpp"
#include "../framebuffer/modelbuffer.hpp"
#include "../framebuffer/framebuffer.hpp"
#include "drawable.hpp"
#include "line.hpp"

#define NULL_OBJ 1
#define PLAYER_OBJ 2
#define ENEMY_OBJ 3
#define LASER_OBJ 4
#define EXPLOSION_OBJ 5
#define PHYSICAL_OBJ 6
#define BOMB_OBJ 7

class Polygon : public Drawable
{
  protected:
    std::vector<Coordinate *> *points;
    Coordinate *anchor;
    color c;
    double scaleFactor;
    double rotation; // rad
    char id;         // polygon identifier
    int zAxis;
    int xfill, yfill;

  public:
    Polygon()
    {
        this->points = new std::vector<Coordinate *>();
        this->c = CWHITE;
        this->scaleFactor = 1;
        this->rotation = 0;
        this->id = NULL_OBJ;
        this->zAxis = 0;
        this->xfill = -1;
        this->yfill = -1;
    }

    Polygon(std::string filename, color c, char id, int zAxis = 0) : Polygon()
    {
        this->zAxis = zAxis;
        std::ifstream f(filename);
        int x, y, minX, maxX, minY, maxY;
        bool first = true;
        while (f >> x >> y)
        {
            if (first)
            {
                minX = maxX = x;
                minY = maxY = y;
            }
            first = false;
            minX = std::min(minX, x);
            maxX = std::max(maxX, x);
            minY = std::min(minY, y);
            maxY = std::max(maxY, y);
            this->addPoint(new Coordinate(x, y));
        }
        f.close();
        this->c = c;
        this->anchor = new Coordinate((minX + maxX) / 2, (minY + maxY) / 2);
        this->id = id;
    }

    ~Polygon()
    {
        for (int i = 0; i < this->points->size(); i++)
        {
            delete this->points->at(i);
        }
        delete this->points;
        delete this->anchor;
    }

    void moveWithoutAnchor(int dx, int dy)
    {
        for (int i = 0; i < this->points->size(); i++)
        {
            this->points->at(i)->move(dx, dy);
        }
    }

    void addPoint(Coordinate *coordinate)
    {
        this->points->push_back(coordinate);
    }

    Coordinate *getAnchor() const
    {
        return this->anchor;
    }

    void setAnchor(int x, int y) {
        this->anchor->setX(x);
        this->anchor->setY(y);
    }

    int getZAxis()
    {
        return this->zAxis;
    }


    void setFillCoor(int x, int y){
        this->xfill = x;
        this->yfill = y;
    }

    void move(int dx, int dy)
    {
        for (int i = 0; i < this->points->size(); i++)
        {
            this->points->at(i)->move(dx, dy);
        }
        this->anchor->move(dx, dy);
    }

    void moveTo(int x, int y, int maxVelocity = -1)
    {
        int dx = x - this->anchor->getX();
        int dy = y - this->anchor->getY();
        int dist2 = dx * dx + dy * dy;
        if (maxVelocity >= 0 && dist2 > maxVelocity && (dx != 0 || dy != 0))
        {
            double factor = maxVelocity * 1.0 / sqrt(dist2);
            dx *= factor;
            dy *= factor;
        }
        this->move(dx, dy);
    }

    void scale(double scaleFactor)
    {
        this->scaleFactor *= scaleFactor;
    }

    void linearScale(double scaleFactor)
    {
        this->scaleFactor += scaleFactor;
    }

    void scaleTo(double scaleFactor, double maxVelocity = 0)
    {
        if (this->scaleFactor > scaleFactor)
        {
            this->scaleFactor = std::max(this->scaleFactor - maxVelocity, scaleFactor);
        }
        else
        {
            this->scaleFactor = std::min(this->scaleFactor + maxVelocity, scaleFactor);
        }
    }

    void rotate(double rotation)
    {
        this->rotation += rotation;
    }

    void rotateTo(double rotation, double maxVelocity = 0)
    {
        if (this->rotation > rotation)
        {
            this->rotation = std::max(this->rotation - maxVelocity, rotation);
        }
        else
        {
            this->rotation = std::min(this->rotation + maxVelocity, rotation);
        }
    }

    double getScaleFactor() const
    {
        return this->scaleFactor;
    }

    double getRotation() const
    {
        return this->rotation;
    }

    char getId() const
    {
        return this->id;
    }

    std::pair<Coordinate *, Coordinate *> *getBoundingBox()
    {
        Coordinate *point = this->points->at(0)->transform(this->scaleFactor, this->rotation, this->anchor);
        int minX = point->getX();
        int maxX = minX;
        int minY = point->getY();
        int maxY = minY;
        delete point;
        for (int i = 1; i < this->points->size(); i++)
        {
            point = this->points->at(i)->transform(this->scaleFactor, this->rotation, this->anchor);
            int x = point->getX();
            int y = point->getY();
            minX = std::min(minX, x);
            maxX = std::max(maxX, x);
            minY = std::min(minY, y);
            maxY = std::max(maxY, y);
            delete point;
        }
        return new std::pair<Coordinate *, Coordinate *>(new Coordinate(minX, minY), new Coordinate(maxX, maxY));
    }

    Coordinate *getCenter()
    {
        std::pair<Coordinate *, Coordinate *> *boundingBox = this->getBoundingBox();
        int x = (boundingBox->first->getX() + boundingBox->second->getX()) / 2;
        int y = (boundingBox->first->getY() + boundingBox->second->getY()) / 2;
        delete boundingBox->first;
        delete boundingBox->second;
        delete boundingBox;
        return new Coordinate(x, y);
    }

    bool isOverlapping(std::pair<Coordinate *, Coordinate *> *otherBoundingBox)
    {
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

    void draw(IFrameBuffer *framebuffer)
    {
        std::pair<Coordinate *, Coordinate *> *boundingBox = this->getBoundingBox();
        int width = boundingBox->second->getX() - boundingBox->first->getX() + 1;
        int height = boundingBox->second->getY() - boundingBox->first->getY() + 1;

        ModelBuffer *oriModelBuffer = new ModelBuffer(width, height, boundingBox->first->getX(), boundingBox->first->getY());
        oriModelBuffer->clearScreen();
        this->drawLines(oriModelBuffer);

        ModelBuffer *modelBuffer = new ModelBuffer(width, height, boundingBox->first->getX(), boundingBox->first->getY());
        modelBuffer->clearScreen();

        this->drawLines(modelBuffer);
        if (this->xfill == -1) {
            modelBuffer->floodfill(this->c);
        } else {
            modelBuffer->floodfill(this->c, new Coordinate(this->xfill, this->yfill));
        }
        // Coordinate *current = new Coordinate(0, 0);
        // for (int y = 0; y < height; y++)
        /**
         * Things to consider:
         * .....#..
         * ...##... line with gradient < 1
         * ..#..... solved.
         *
         * ..#.....
         * ..##.... sharp line
         * ..#.#... not solved yet.
         * ..#..#..
         */

        // for (int y = boundingBox->first->getY(); y < boundingBox->second->getY(); y++)
        // {
        //     bool fill = false;

        //     for (int x = boundingBox->first->getX(); x < boundingBox->second->getX(); x++)
        //     {
        //         if (oriModelBuffer->lazyCheck(x, y) != CBLACK)
        //         {
        //             bool topIsFilled = oriModelBuffer->lazyCheck(x - 1, y - 1) != CBLACK;
        //             bool bottomIsFilled = oriModelBuffer->lazyCheck(x - 1, y + 1) != CBLACK;

        //             topIsFilled |= oriModelBuffer->lazyCheck(x, y - 1) != CBLACK;
        //             bottomIsFilled |= oriModelBuffer->lazyCheck(x, y + 1) != CBLACK;

        //             while (oriModelBuffer->lazyCheck(x + 1, y) != CBLACK)
        //             {
        //                 x++;
        //                 topIsFilled |= oriModelBuffer->lazyCheck(x, y - 1) != CBLACK;
        //                 bottomIsFilled |= oriModelBuffer->lazyCheck(x, y + 1) != CBLACK;
        //             }

        //             topIsFilled |= oriModelBuffer->lazyCheck(x + 1, y - 1) != CBLACK;
        //             bottomIsFilled |= oriModelBuffer->lazyCheck(x + 1, y + 1) != CBLACK;

        //             if (topIsFilled && bottomIsFilled)
        //             {
        //                 fill = !fill;
        //             }
        //         }
        //         if (fill)
        //         {
        //             modelBuffer->lazyDraw(x, y, this->c);
        //         }
        //     }
        // }
        // if (this->id == PLAYER_OBJ) // turn on this to debug
        // {
        //     oriModelBuffer->debug();
        //     modelBuffer->debug();
        //     exit(0);
        // }

        FrameBuffer *realframebuffer = (FrameBuffer *)framebuffer;
        modelBuffer->flush(realframebuffer, this->zAxis);

        delete modelBuffer;
        delete oriModelBuffer;
    }

    void drawLines(ModelBuffer *modelBuffer)
    {
        int nLines = this->points->size();

        for (int i = 0; i < nLines; i++)
        {
            Coordinate *c1 = this->points->at(i)->transform(this->scaleFactor, this->rotation, this->anchor);
            Coordinate *c2 = this->points->at((i + 1) % nLines)->transform(this->scaleFactor, this->rotation, this->anchor);
            Line *line = new Line(c1->getX(), c1->getY(), c2->getX(), c2->getY(), this->c, this->c);
            line->draw(modelBuffer);
            delete c1;
            delete c2;
            delete line;
        }
    }
};

#endif