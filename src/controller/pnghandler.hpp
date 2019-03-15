#include <curses.h>
#include <stdlib.h>

#include <png.h>
#include "../etc/image.hpp"

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