#include "common.h"

#ifdef __APPLE__
void take_screenshot();
#else
void take_screenshot();
#endif

int main() {
    take_screenshot();
    return 0;
}

