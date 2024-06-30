#ifdef __APPLE__
#include <ApplicationServices/ApplicationServices.h>
#include <CoreGraphics/CoreGraphics.h>
#include <ImageIO/ImageIO.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    char time_buffer[40];
    char buffer[120];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_buffer, sizeof(time_buffer), "%Y%m%d%H%M%S", timeinfo);
    snprintf(buffer, sizeof(buffer), "screenshots/screenshot_%s.png", time_buffer);

    CFStringRef path = CFStringCreateWithCString(NULL, buffer, kCFStringEncodingUTF8);
    CFURLRef url = CFURLCreateWithFileSystemPath(NULL, path, kCFURLPOSIXPathStyle, false);
    CFRelease(path);

    CFStringRef type = CFStringCreateWithCString(NULL, "public.png", kCFStringEncodingUTF8);
    CGImageDestinationRef destination = CGImageDestinationCreateWithURL(url, type, 1, NULL);
    CFRelease(url);
    CFRelease(type);
    if (!destination) {
        fprintf(stderr, "Failed to create image destination\n");
        CGImageRelease(screenshot);
        return;
    }
    CGImageDestinationAddImage(destination, screenshot, NULL);
    if (!CGImageDestinationFinalize(destination)) {
        fprintf(stderr, "Failed to write image to file\n");
    }
    CFRelease(destination);
    CGImageRelease(screenshot);
    CGContextRelease(context);
    free(image_data);
}
#endif

