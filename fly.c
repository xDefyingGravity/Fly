#include "fly.h"
#include <stdlib.h>
#include <stdio.h>
#include <png.h>

void initImage(Image *image) {
    image->width = WIDTH;
    image->height = HEIGHT;
    image->pixels = (Pixel **)malloc(HEIGHT * sizeof(Pixel *));
    if (image->pixels == NULL) {
        perror("Error allocating memory for image pixels");
        exit(EXIT_FAILURE);
    }
    for (int y = 0; y < HEIGHT; y++) {
        image->pixels[y] = (Pixel *)malloc(WIDTH * sizeof(Pixel)); 
        if (image->pixels[y] == NULL) {
            perror("Error allocating memory for image row");
            exit(EXIT_FAILURE);
        }
        for (int x = 0; x < WIDTH; x++) {
            image->pixels[y][x] = (Pixel){0, 0, 0};
        }
    }
}

Image readPNG(const char* filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Error reading input file");
        exit(EXIT_FAILURE);
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    if (bit_depth == 16) png_set_strip_16(png);
    if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    Image img;
    img.width = width;
    img.height = height;
    img.pixels = (Pixel **)malloc(height * sizeof(Pixel *));
    for (int y = 0; y < height; y++) {
        img.pixels[y] = (Pixel *)malloc(width * sizeof(Pixel));
    }

    png_bytep row_pointers[height];
    for (int y = 0; y < height; y++) {
        row_pointers[y] = (png_bytep)malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, row_pointers);

    for (int y = 0; y < height; y++) {
        png_bytep row = row_pointers[y];
        for (int x = 0; x < width; x++) {
            png_bytep px = &(row[x * 4]);
            img.pixels[y][x].r = px[0];
            img.pixels[y][x].g = px[1];
            img.pixels[y][x].b = px[2];
        }
    }

    for (int y = 0; y < height; y++) {
        free(row_pointers[y]);
    }

    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);

    return img;
}

void writePNG(const char* filename, Image img) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, NULL);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    png_init_io(png, fp);
    png_set_IHDR(png, info, img.width, img.height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);

    for (int y = 0; y < img.height; y++) {
        png_write_row(png, (png_const_bytep)img.pixels[y]);
    }

    png_write_end(png, NULL);
    png_destroy_write_struct(&png, &info);
    fclose(fp);
}

Pixel toGrayscale(Pixel pixel) {
    int r = (int)(0.299 * pixel.r);
    int g = (int)(0.587 * pixel.g);
    int b = (int)(0.114 * pixel.b);

    int gray = r + g + b;
    return (Pixel){gray, gray, gray};
}

int grayscale(char* filename, char* output) {
    Image img = readPNG(filename);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img.pixels[y][x] = toGrayscale(img.pixels[y][x]);
        }
    }

    writePNG(output, img);

    return 0;
}
