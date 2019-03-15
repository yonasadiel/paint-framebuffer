#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <iostream>
#include <fstream>

struct Pixel {
    int red;
    int green;
    int blue;
};

class Image {
private:
    struct Pixel** pixels;
    int height;
    int width;

public:
    Image(char* filename) : Image(filename, 16, 16) {};
    Image(char* filename, int height, int width) {
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
};

#endif