#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include "util.h"

#define WIDTH 256
#define HEIGHT WIDTH
#define NUM_PIXELS (WIDTH * HEIGHT * 3)

int main()
{
    bitmap_t *bitmap;
        
    if (0 != init_bitmap(&bitmap, WIDTH, HEIGHT)) {
        return -1;
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            set_pixel(bitmap, i, j, 0xFF - i, 0xFF - j, i);
        }
    }

    if (0 != save_bitmap(bitmap, "img.bmp")) {
        printf("Failed to save bitmap!\n");
    }

    return free_bitmap(&bitmap);
}
