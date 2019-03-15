#ifndef ANIMATED
#define ANIMATED

#include <vector>
#include <stdlib.h>

#include "polygon.hpp"
#include "../etc/coordinate.hpp"

class Animated : public Polygon
{
  protected:
    bool loop;
    bool hidden;
    bool hiddenAfterFinish;
    bool animationRunning;

    int maxAnchorVelocity;
    int nextAnchorKeyframes;
    std::vector<Coordinate *> *anchorKeyframes;

    double maxScaleVelocity;
    int nextScaleKeyframes;
    std::vector<double> *scaleKeyframes;

    double maxRotationVelocity;
    int nextRotationKeyframes;
    std::vector<double> *rotationKeyframes;

  public:
    // Animated(bool start = true, 
    //          bool loop = false,
    //          int maxAnchorVelocity = 0,
    //          double maxScaleVelocity = 0,
    //          double maxRotationVelocity = 0) {
    //     initAttributes(start, loop, maxAnchorVelocity, maxScaleVelocity, maxRotationVelocity);
        
    // }

    Animated(std::string filename, color c, char id, bool start = false, bool loop = false,
             int maxAnchorVelocity = 0,
             double maxScaleVelocity = 0,
             double maxRotationVelocity = 0) : 
    Polygon(filename, c, id)
    {
        initAttributes(start, loop, maxAnchorVelocity, maxScaleVelocity, maxRotationVelocity);
    }

    Animated(std::vector<Coordinate*>* points, color c, char id, bool start = false, bool loop = false,
             int maxAnchorVelocity = 0,
             double maxScaleVelocity = 0,
             double maxRotationVelocity = 0) : 
    Polygon(points, c, id)
    {
        initAttributes(start, loop, maxAnchorVelocity, maxScaleVelocity, maxRotationVelocity);
    }

    ~Animated()
    {
        for (int i = 0; i < this->anchorKeyframes->size(); i++)
        {
            delete this->anchorKeyframes->at(i);
        }
        delete this->anchorKeyframes;
        delete this->scaleKeyframes;
        delete this->rotationKeyframes;
    }

    void initAttributes(bool start = true, bool loop = false,
             int maxAnchorVelocity = 0,
             double maxScaleVelocity = 0,
             double maxRotationVelocity = 0) 
    {
        this->loop = loop;
        this->hidden = false;
        this->hiddenAfterFinish = false;
        this->animationRunning = start;        

        this->maxAnchorVelocity = maxAnchorVelocity;
        this->nextAnchorKeyframes = 0;
        this->anchorKeyframes = new std::vector<Coordinate *>();

        this->maxScaleVelocity = maxScaleVelocity;
        this->nextScaleKeyframes = 0;
        this->scaleKeyframes = new std::vector<double>();

        this->maxRotationVelocity = maxRotationVelocity;
        this->nextRotationKeyframes = 0;
        this->rotationKeyframes = new std::vector<double>();
    }

    void startAnimation(bool loop = true, int maxAnchorVelocity = 5, double maxScaleVelocity=1, double maxRotationVelocity=1){
        this->animationRunning = true;
        this->loop = loop;
        this->maxAnchorVelocity = maxAnchorVelocity;
        this->maxScaleVelocity = maxScaleVelocity;
        this->maxRotationVelocity = maxRotationVelocity;
    }

    void addAnchorKeyframe(Coordinate *anchor)
    {
        if (this->anchorKeyframes->empty())
        {
            this->moveTo(anchor->getX(), anchor->getY());
            this->nextAnchorKeyframes = 1;
        }
        this->anchorKeyframes->push_back(anchor);
    }

    void addScaleKeyframe(double scale)
    {
        if (this->scaleKeyframes->empty())
        {
            this->scale(scale);
            this->nextScaleKeyframes = 1;
        }
        this->scaleKeyframes->push_back(scale);
    }

    void addRotationKeyframe(double rotation)
    {
        if (this->rotationKeyframes->empty())
        {
            this->rotate(rotation);
            this->nextRotationKeyframes = 1;
        }
        this->rotationKeyframes->push_back(rotation);
    }

    void hide()
    {
        this->hidden = true;
    }

    bool isHidden() {
        return this->hidden;
    }

    void hideAfterFinished()
    {
        this->hiddenAfterFinish = true;
    }

    void animate()
    {
        if (!this->animationRunning) {
            return;
        }
        if (!this->anchorKeyframes->empty() && this->anchorKeyframes->size() > this->nextAnchorKeyframes)
        {
            int fromX = this->anchor->getX();
            int fromY = this->anchor->getY();
            Coordinate *dest = this->anchorKeyframes->at(this->nextAnchorKeyframes);
            this->moveTo(dest->getX(), dest->getY(), this->maxAnchorVelocity);
            if (abs(dest->getX() - fromX) < this->maxAnchorVelocity && abs(dest->getY() - fromY) < this->maxAnchorVelocity)
            {
                this->nextAnchorKeyframes++;
                if (this->loop)
                {
                    this->nextAnchorKeyframes %= this->anchorKeyframes->size();
                }
            }
        }

        if (!this->scaleKeyframes->empty() && this->scaleKeyframes->size() > this->nextScaleKeyframes)
        {
            double from = this->getScaleFactor();
            double to = this->scaleKeyframes->at(this->nextScaleKeyframes);
            this->scaleTo(to, this->maxScaleVelocity);
            if (from == this->getScaleFactor())
            {
                this->nextScaleKeyframes++;
                if (this->loop)
                {
                    this->nextScaleKeyframes %= this->scaleKeyframes->size();
                }
            }
        }

        if (!this->rotationKeyframes->empty() && this->rotationKeyframes->size() > this->nextRotationKeyframes)
        {
            double from = this->getRotation();
            double to = this->rotationKeyframes->at(this->nextRotationKeyframes);
            this->rotateTo(to, this->maxRotationVelocity);
            if (from == this->getRotation())
            {
                this->nextRotationKeyframes++;
                if (this->loop)
                {
                    this->nextRotationKeyframes %= this->rotationKeyframes->size();
                }
            }
        }
        if (this->nextAnchorKeyframes == this->anchorKeyframes->size() &&
            this->nextRotationKeyframes == this->rotationKeyframes->size() &&
            this->nextScaleKeyframes == this->scaleKeyframes->size())
        {
            this->hide();
        }
    }

    void draw(IFrameBuffer *framebuffer)
    {
        if (!this->hidden)
        {
            Polygon::draw(framebuffer);
        }
    }
};

#endif