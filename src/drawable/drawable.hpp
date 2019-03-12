#ifndef DRAWABLE
#define DRAWABLE

#include "../framebuffer/framebuffer.hpp"

class Drawable {
  public:
    virtual void draw(IFrameBuffer *framebuffer) = 0;
    virtual void animate() {}
    virtual char getId() const { return 0; }
};

#endif