#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <iostream>
#include <fstream>

#include "drawable.hpp"


class Image {
private:
    int height;
    int width;
    char id;

public:
    unsigned int** pixels;

    Image(std::string filename, int height = 16, int width = 16) {
        std::ifstream f(filename);
        unsigned int b, g, r;

        this->height = height;
        this->width = width;

        pixels = new unsigned int*[width];
        for (int i = 0; i < width; i++) {
            pixels[i] = new unsigned int[height];
        }
    
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				f >> r >> g >> b;
				pixels[j][i] = ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
			}
		}
        f.close();
    }

    int getHeight() {
        return height;
    }

    int getWidth() {
        return width;
    }

    void printPixel() {
        for (int i = 0; i < width; i++)
            for (int j = 0; j < height; j++)
                std::cout << pixels[i][j] << std::endl;
    }
};

#endif