#ifndef IMAGE_STORE_H
#define IMAGE_STORE_H

#include <stddef.h>
#include "esp_system.h"

#ifdef __cplusplus
extern "C" {
#endif

void image_store_init(void);
void save_last_detection_image(uint8_t *jpeg_image, size_t jpeg_img_size);
uint8_t* get_last_detection_image(size_t *jpeg_img_size);
void save_most_recent_image(uint8_t *jpeg_image, size_t jpeg_img_size);
uint8_t* get_most_recent_image(size_t *jpeg_img_size);

#ifdef __cplusplus
}
#endif

#endif // IMAGE_STORE_H