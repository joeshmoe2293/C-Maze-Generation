#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"
#include "util.h"

int init_bitmap(bitmap_t **bitmap, uint32_t width, uint32_t height)
{
    if (NULL == bitmap ||
        NULL == (*bitmap = malloc(sizeof(bitmap_t)))) {
        return -1;
    }

    memset(*bitmap, 0, sizeof(bitmap_t));

    if (NULL == ((*bitmap)->map = (uint8_t *)malloc(width * height * 3))) {
        return -1;
    }

    // Set up the regular header
    (*bitmap)->header.sig[0] = 'B';
    (*bitmap)->header.sig[1] = 'M';
    (*bitmap)->header.file_size = (width * height) * 3 + sizeof(bitmap_t);
    (*bitmap)->header.data_offset = sizeof((*bitmap)->header) + \
                                    sizeof((*bitmap)->info_header);

    // Set up the info header
    (*bitmap)->info_header.size = 40;
    (*bitmap)->info_header.width = width;
    (*bitmap)->info_header.height = height;
    (*bitmap)->info_header.planes = 1;
    (*bitmap)->info_header.bits_per_pixel = 24;
    (*bitmap)->info_header.compression = 0; // TODO consider adding
    (*bitmap)->info_header.image_size = 0;
    (*bitmap)->info_header.x_pixels_per_m = 0;
    (*bitmap)->info_header.y_pixels_per_m = 0;
    (*bitmap)->info_header.colors_used = 0;
    (*bitmap)->info_header.important_colors = 0;

    return 0;
}

int set_pixel(bitmap_t *bitmap, int row, int col,
              uint8_t r, uint8_t g, uint8_t b)
{
    int pixel_idx;
    int map_height;

    if (bitmap == NULL || bitmap->map == NULL ||
        row < 0 || col < 0 ||
        row >= bitmap->info_header.height ||
        col >= bitmap->info_header.width) {
        return -1;
    }

    map_height = bitmap->info_header.height;

    pixel_idx = ((map_height - row - 1) * map_height + col) * 3;
    bitmap->map[pixel_idx + 0] = b;
    bitmap->map[pixel_idx + 1] = g;
    bitmap->map[pixel_idx + 2] = r;

    return 0;
}

int save_bitmap(bitmap_t *bitmap, const char *filename)
{
    int status = -1;
    FILE *file;

    if (bitmap != NULL && bitmap->map != NULL) {
        file = fopen(filename, "w");

        if (NULL == file) {
            printf("Failed to open to file\n");

            return -1;
        }

        fwrite(&bitmap->header, 1, sizeof(bitmap->header), file);
        fwrite(&bitmap->info_header, 1, sizeof(bitmap->info_header), file);
        fwrite(bitmap->map, 1,
               bitmap->info_header.width * bitmap->info_header.height * 3,
               file);

        status = fclose(file);
    }
   
    return status;
}

int free_bitmap(bitmap_t **bitmap)
{
    if (bitmap == NULL || *bitmap == NULL || (*bitmap)->map == NULL) {
        return -1;
    }

    free((*bitmap)->map);
    free((*bitmap));
    *bitmap = NULL;

    return 0;
}
