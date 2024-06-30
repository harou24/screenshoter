#ifndef __APPLE__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../lib/common.h"

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

