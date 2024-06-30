#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

void save_png(const char *filename, unsigned char *image_data, int width, int height) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Could not open file for writing\n");
        return;
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fprintf(stderr, "Could not create PNG write struct\n");
        fclose(fp);
        return;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        fprintf(stderr, "Could not create PNG info struct\n");
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        return;
    }

    if (setjmp(png_jmpbuf(png))) {
        fprintf(stderr, "PNG creation error\n");
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        return;
    }

    png_init_io(png, fp);
    png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_bytep row_pointers[height];
    for (int y = 0; y < height; y++) {
        row_pointers[y] = image_data + y * width * 3;
    }
    png_set_rows(png, info, row_pointers);
    png_write_png(png, info, PNG_TRANSFORM_IDENTITY, NULL);

    png_destroy_write_struct(&png, &info);
    fclose(fp);
}

