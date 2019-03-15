#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <iostream>
#include <fstream>

#include "drawable.hpp"

struct Pixel {
    int red;
    int green;
    int blue;
};

class Image : public Drawable {
private:
    struct Pixel** pixels;
    int height;
    int width;
    char id;

public:
    Image(char* filename, int height = 16, int width = 16, char id = 0) {
        std::ifstream f(filename);
        int b, g, r;

        this->height = height;
        this->width = width;

        pixels = new Pixel*[height];
        for (int i = 0; i < height; i++) {
            pixels[i] = new Pixel[width];
        }
    
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				f >> b >> g >> r;
				pixels[i][j].blue = b;
                pixels[i][j].green = g;
                pixels[i][j].red = r;
			}
		}
        f.close();
    }

    char getId const {
        return this->id;
    }

    void draw(IFrameBuffer *framebuffer) {

    }

    void animate() {
        
    }
};

#endif