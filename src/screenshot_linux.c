#ifndef __APPLE__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void take_screenshot() {
    Display* display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        return;
    }

    Screen* screen = DefaultScreenOfDisplay(display);
    int screenNumber = DefaultScreen(display);
    int width = screen->width;
    int height = screen->height;
    Window root = RootWindow(display, screenNumber);
    XImage* img = XGetImage(display, root, 0, 0, width, height, AllPlanes, ZPixmap);

    if (!img) {
        fprintf(stderr, "Failed to capture screenshot\n");
        XCloseDisplay(display);
        return;
    }

    unsigned char *image_data = (unsigned char *)malloc(3 * width * height);
    if (!image_data) {
        fprintf(stderr, "Failed to allocate memory\n");
        XDestroyImage(img);
        XCloseDisplay(display);
        return;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned long pixel = XGetPixel(img, x, y);
            image_data[(y * width + x) * 3 + 0] = (pixel & img->red_mask) >> 16;
            image_data[(y * width + x) * 3 + 1] = (pixel & img->green_mask) >> 8;
            image_data[(y * width + x) * 3 + 2] = (pixel & img->blue_mask);
        }
    }

    time_t rawtime;
    struct tm *timeinfo;
    char time_buffer[40];
    char buffer[120];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_buffer, sizeof(time_buffer), "%Y%m%d%H%M%S", timeinfo);
    snprintf(buffer, sizeof(buffer), "screenshots/screenshot_%s.png", time_buffer);

    save_png(buffer, image_data, width, height);

    free(image_data);
    XDestroyImage(img);
    XCloseDisplay(display);
}
#endif

