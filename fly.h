#ifndef __FLY__H__
#define __FLY__H__


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <png.h>

#define WIDTH  640
#define HEIGHT 480
#define min(a, b) ((a) < (b) ? (a) : (b))

extern int* COMBINE_STRENGTH;


typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Pixel;


typedef struct {
    int width, height;
    Pixel **pixels;
} Image;


void initImage(Image *image);
Image readPNG(const char* filename);
void writePNG(const char* filename, Image img);
int grayscale(char* filename, char* output);


#endif