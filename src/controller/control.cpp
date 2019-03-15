#include <curses.h>
#include <vector>
#include <iostream>
#include <png.h>

#include "paint.hpp"
#include "control.hpp"
#include "../etc/image.hpp"

#define STRING_BUFFER_SIZE 256

char* readLine() {
    char* buffer = new char[STRING_BUFFER_SIZE];
    for (int i = 0; i < STRING_BUFFER_SIZE; i++) {
        buffer[i] = 0;
    }

    int idx = 0;
    while (true) {
        char x = getchar();
        if (x == 127 && idx > 0) {
            std::cout << "\b \b";
            idx--;
            buffer[idx] = 0;
        } if (x == 13) {
            std::cout << "\n";
            break;
        } else if (idx < STRING_BUFFER_SIZE &&
            ((x >= 'A' && x <= 'Z') || 
            (x >= 'a' && x <= 'z') || 
            (x >= '0' && x <= '9') ||
            x == '-' || x == '_' || x == '/' || x == '.')) {
            buffer[idx] = x;
            idx++;
            std::cout << x;
        }
    }
    
    return buffer;
}

image mImage;

image readPNG(char *filename) {
    int width, height;
    png_byte color_type;
    png_byte bit_depth;
    png_bytep *row_pointers;
    pixel** pixels;

    FILE *fp = fopen(filename, "rb");
    if (!fp) abort();

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png) abort();

    png_infop info = png_create_info_struct(png);
    if(!info) abort();

    if(setjmp(png_jmpbuf(png))) abort();

    png_init_io(png, fp);
    png_read_info(png, info);

    width      = png_get_image_width(png, info);
    height     = png_get_image_height(png, info);
    color_type = png_get_color_type(png, info);
    bit_depth  = png_get_bit_depth(png, info);

    if(bit_depth == 16) 
        png_set_strip_16(png);
    if(color_type == PNG_COLOR_TYPE_PALETTE) 
        png_set_palette_to_rgb(png);
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);
    if(png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);
    if(color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    if(color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for(int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
    }

    png_read_image(png, row_pointers);
    png_read_end(png, NULL);

    pixels = new pixel*[height];
    for(int y = 0; y < height; y++) {
        png_bytep row = row_pointers[y];
        pixels[y] = new pixel[width];
        for(int x = 0; x < width; x++) {
            png_bytep px = &(row[x * 4]);
            pixels[y][x].red = px[0];
            pixels[y][x].green = px[1];
            pixels[y][x].blue = px[2];
            pixels[y][x].alpha = px[3];
        }
    }

    image img;
    img.pixels = pixels;
    img.width = width;
    img.height = height;

    fclose(fp);

    return img;
}

void writePNG(char *filename, image img) {
    FILE *fp = fopen(filename, "wb");
    if(!fp) abort();

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) abort();

    png_infop info = png_create_info_struct(png);
    if (!info) abort();

    if (setjmp(png_jmpbuf(png))) abort();

    png_init_io(png, fp);

    png_set_IHDR(
        png,
        info,
        img.width, img.height,
        8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info);

    png_bytep row = (png_bytep) malloc(4 * img.width * sizeof(png_byte));
    int x, y;
    for (y = 0; y < img.height ; y++) {
        for (x = 0 ; x < img.width ; x++) {
            row[x*4 + 0] = img.pixels[y][x].red;
            row[x*4 + 1] = img.pixels[y][x].green;
            row[x*4 + 2] = img.pixels[y][x].blue;
            row[x*4 + 3] = 0xFF;
        }
        png_write_row(png, row);
    }
    png_write_end(png, NULL);

    fclose(fp);
}

void readInput(Paint* paint) {
    initscr();
    timeout(-1);
    noecho();
    cbreak();

    int cursorSpeed = 5;
    int panSpeed = 5;
    unsigned char state = STATE_IDLE;

    while (paint->stillRunning()) {
        if (!paint->isTextMode()) {
            char c = getch();
            // we use nextState so it wont execute next interpretation before the state is changed
            unsigned char nextState = state;
            // printf("\n%d\n", 0+c);

            switch (c) {
                case CHOOSE_COLOR_1:
                    paint->setFillColor(CWHITE);
                    break;
                case CHOOSE_COLOR_2:
                    paint->setFillColor(CRED);
                    break;
                case CHOOSE_COLOR_3:
                    paint->setFillColor(CGREEN);
                    break;
                case CHOOSE_COLOR_4:
                    paint->setFillColor(CBLUE);
                    break;
                case CHOOSE_COLOR_5:
                    paint->setFillColor(CYELLOW);
                    break;
                case CHOOSE_COLOR_6:
                    paint->setFillColor(CCYAN);
                    break;
                case CHOOSE_COLOR_7:
                    paint->setFillColor(CMAGENTA);
            }


            if (paint->isIdle()) {
                if (c == COMMAND_QUIT) {
                    paint->terminate();
                } else if (c == COMMAND_DRAW_RECTANGLE) {
                    paint->showCursor();
                    paint->startDrawRectangle();
                } else if (c == COMMAND_DRAW_TRIANGLE) {
                    paint->showCursor();
                    paint->startDrawTriangle();
                } else if (c == COMMAND_DRAW_LINE) {
                    paint->showCursor();
                    paint->startDrawLine();
                } else if (c == COMMAND_SELECT) {
                    paint->showCursor();
                    paint->startSelection();
                } else if (c == COMMAND_SAVE) {
                    paint->startSaving();
                } else if (c == COMMAND_LOAD) {
                    paint->startLoading();
                } else if (c == COMMAND_PAN_LEFT) {
                    paint->panScreen(-panSpeed, 0);
                } else if (c == COMMAND_PAN_RIGHT) {
                    paint->panScreen(panSpeed, 0);
                } else if (c == COMMAND_PAN_UP) {
                    paint->panScreen(0, -panSpeed);
                } else if (c == COMMAND_PAN_DOWN) {
                    paint->panScreen(0, panSpeed);
                }
            }

            if (paint->isAbleToMoveCursor()) {
                if (c == COMMAND_CURSOR_MOVE_LEFT) {
                    paint->moveCursor(-cursorSpeed, 0);
                } else if (c == COMMAND_CURSOR_MOVE_RIGHT) {
                    paint->moveCursor(cursorSpeed, 0);
                } else if (c == COMMAND_CURSOR_MOVE_UP) {
                    paint->moveCursor(0, -cursorSpeed);
                } else if (c == COMMAND_CURSOR_MOVE_DOWN) {
                    paint->moveCursor(0, cursorSpeed);
                } else if (c == COMMAND_DELETE) {
                    paint->deleteWorkingPolygon();
                }
            }

            if (c == COMMAND_ENTER) {
                paint->handleClick();
            }

            if (paint->isObjectSelected()) {
                if (c == COMMAND_ROTATE_RIGHT) {
                    paint->rotateRight();
                } else if (c == COMMAND_ROTATE_LEFT) {
                    paint->rotateLeft();
                } else if (c == COMMAND_SCALE_UP) {
                    paint->scaleUp();
                } else if (c == COMMAND_SCALE_DOWN) {
                    paint->scaleDown();
                }
            }

            paint->goToNextState();
        } else {
            clear();
            for (int i = 0; i < 200; i++) 
                std::cout << "\n";

            if (paint->isSaving()) {
                std::cout << "\bEnter filename: ";
                char* filename = readLine();
                std::cout << "\rSaving to file " << filename << "....\n\r";

                image img = paint->getSnapshot();

                writePNG(filename, img);

                std::cout << "Successfully Saved!\n\r";
                std::cout << "Press any key to continue...";
                getchar();
            } else if (paint->isLoading()) {
                std::cout << "\bEnter filename: ";
                char* filename = readLine();
                std::cout << "\rLoading from file " << filename << "....\n\r";
                image img = readPNG(filename);
                mImage = img;
                
                Rasterized* r = new Rasterized(img.pixels, img.width, img.height, 0, 0);
                paint->pushDrawable(r);
            }

            paint->exitTextMode();
        }
    }
    endwin();
}
