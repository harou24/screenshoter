#ifdef __APPLE__
#include <ApplicationServices/ApplicationServices.h>
#include <CoreGraphics/CoreGraphics.h>
#include <time.h>
#include "common.h"

void take_screenshot() {
    CGDirectDisplayID displayID = CGMainDisplayID();
    CGImageRef screenshot = CGDisplayCreateImage(displayID);
    if (!screenshot) {
        fprintf(stderr, "Failed to capture screenshot\n");
        return;
    }

    size_t width = CGImageGetWidth(screenshot);
    size_t height = CGImageGetHeight(screenshot);
    unsigned char *image_data = (unsigned char *)malloc(3 * width * height);
    if (!image_data) {
        fprintf(stderr, "Failed to allocate memory\n");
        CGImageRelease(screenshot);
        return;
    }

    CGContextRef context = CGBitmapContextCreate(image_data, width, height, 8, 3 * width, CGColorSpaceCreateDeviceRGB(), kCGImageAlphaNoneSkipLast);
    CGContextDrawImage(context, CGRectMake(0, 0, width, height), screenshot);

    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "screenshot_%Y%m%d%H%M%S.png", timeinfo);

    save_png(buffer, image_data, width, height);

    CGContextRelease(context);
    CGImageRelease(screenshot);
    free(image_data);
}
#endif

