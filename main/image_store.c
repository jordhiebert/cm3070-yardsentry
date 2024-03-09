#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "image_store.h"
#include <string.h>

static const char *TAG = "image_store";

// JPEG image data and its size
static uint8_t *s_jpeg_image = NULL;
static size_t s_jpeg_img_size = 0;
static SemaphoreHandle_t s_mutex = NULL;

// Initialize resources
void image_store_init(void)
{
    s_mutex = xSemaphoreCreateMutex();
}

// Save the latest JPEG image
void save_jpeg_image(uint8_t *jpeg_image, size_t jpeg_img_size)
{
    if (xSemaphoreTake(s_mutex, portMAX_DELAY))
    {
        if (s_jpeg_image)
        {
            free(s_jpeg_image);
            s_jpeg_image = NULL;
            s_jpeg_img_size = 0;
        }

        s_jpeg_image = malloc(jpeg_img_size);
        if (s_jpeg_image == NULL)
        {
            ESP_LOGE(TAG, "Failed to allocate memory for new image");
        }
        else
        {
            memcpy(s_jpeg_image, jpeg_image, jpeg_img_size);
            s_jpeg_img_size = jpeg_img_size;
        }

        xSemaphoreGive(s_mutex);
    }
    else
    {
        ESP_LOGE(TAG, "JPEG image mutex error");
    }
}

// Get the latest JPEG image
// Note: Caller must free the returned pointer after use
uint8_t *get_jpeg_image(size_t *jpeg_img_size)
{
    uint8_t *image_copy = NULL;

    if (s_mutex == NULL)
    {
        ESP_LOGE(TAG, "JPEG image mutex not initialized");
        return NULL;
    }

    xSemaphoreTake(s_mutex, portMAX_DELAY);
    if (s_jpeg_image != NULL && s_jpeg_img_size > 0)
    {
        image_copy = malloc(s_jpeg_img_size);
        if (image_copy != NULL)
        {
            memcpy(image_copy, s_jpeg_image, s_jpeg_img_size);
            *jpeg_img_size = s_jpeg_img_size;
        }
    }
    xSemaphoreGive(s_mutex);

    return image_copy;
}
