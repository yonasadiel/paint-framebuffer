#ifndef RENDERER
#define RENDERER

#include <vector>

#include "../framebuffer/framebuffer.hpp"
#include "../drawable/drawable.hpp"
#include "renderer.hpp"

void draw(FrameBuffer *framebuffer, std::vector<Drawable *> *objects, bool *run);

#endif
