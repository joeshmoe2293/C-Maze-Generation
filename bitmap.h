#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <stdint.h>
#include <stdlib.h>

typedef struct __attribute__((packed)) {
    struct __attribute__((packed)) {
        uint8_t sig[2];
        uint32_t file_size;
        uint32_t reserved1;
        uint32_t data_offset;
    } header;
    struct __attribute__((packed)) {
        uint32_t size;
        uint32_t width;
        uint32_t height;
        uint16_t planes;
        uint16_t bits_per_pixel;
        uint32_t compression;
        uint32_t image_size;
        uint32_t x_pixels_per_m;
        uint32_t y_pixels_per_m;
        uint32_t colors_used;
        uint32_t important_colors;
    } info_header;
    uint8_t *map;
} bitmap_t;

int init_bitmap(bitmap_t **bitmap, uint32_t width, uint32_t height);
int set_pixel(bitmap_t *bitmap, int row, int col,
              uint8_t r, uint8_t g, uint8_t b);
int save_bitmap(bitmap_t *bitmap, const char *filename);
int free_bitmap(bitmap_t **bitmap);

#endif
