#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "image_store.h"
#include <string.h>

static const char *TAG = "image_store";

static uint8_t *s_last_detection_image = NULL;
static size_t s_last_detection_image_size = 0;
static SemaphoreHandle_t s_last_detection_image_mutex = NULL;

static uint8_t *s_most_recent_image = NULL;
static size_t s_most_recent_image_size = 0;
static SemaphoreHandle_t s_most_recent_image_mutex = NULL;

// Initialize resources
void image_store_init(void)
{
    s_last_detection_image_mutex = xSemaphoreCreateMutex();
    s_most_recent_image_mutex = xSemaphoreCreateMutex();
}

// Save the latest JPEG image
void save_last_detection_image(uint8_t *jpeg_image, size_t jpeg_img_size)
{
    if (xSemaphoreTake(s_last_detection_image_mutex, portMAX_DELAY))
    {
        if (s_last_detection_image)
        {
            free(s_last_detection_image);
            s_last_detection_image = NULL;
            s_last_detection_image_size = 0;
        }

        s_last_detection_image = malloc(jpeg_img_size);
        if (s_last_detection_image == NULL)
        {
            ESP_LOGE(TAG, "Failed to allocate memory for new image");
        }
        else
        {
            memcpy(s_last_detection_image, jpeg_image, jpeg_img_size);
            s_last_detection_image_size = jpeg_img_size;
        }

        xSemaphoreGive(s_last_detection_image_mutex);
    }
    else
    {
        ESP_LOGE(TAG, "JPEG image mutex error");
    }
}

// Get the latest JPEG image
// Note: Caller must free the returned pointer after use
uint8_t *get_last_detection_image(size_t *jpeg_img_size)
{
    uint8_t *image_copy = NULL;

    if (s_last_detection_image_mutex == NULL)
    {
        ESP_LOGE(TAG, "JPEG image mutex not initialized");
        return NULL;
    }

    xSemaphoreTake(s_last_detection_image_mutex, portMAX_DELAY);
    if (s_last_detection_image != NULL && s_last_detection_image_size > 0)
    {
        image_copy = malloc(s_last_detection_image_size);
        if (image_copy != NULL)
        {
            memcpy(image_copy, s_last_detection_image, s_last_detection_image_size);
            *jpeg_img_size = s_last_detection_image_size;
        }
    }
    xSemaphoreGive(s_last_detection_image_mutex);

    return image_copy;
}

// Save the latest JPEG image
void save_most_recent_image(uint8_t *jpeg_image, size_t jpeg_img_size)
{
    if (xSemaphoreTake(s_most_recent_image_mutex, portMAX_DELAY))
    {
        if (s_most_recent_image)
        {
            free(s_most_recent_image);
            s_most_recent_image = NULL;
            s_most_recent_image_size = 0;
        }

        s_most_recent_image = malloc(jpeg_img_size);
        if (s_most_recent_image == NULL)
        {
            ESP_LOGE(TAG, "Failed to allocate memory for new image");
        }
        else
        {
            memcpy(s_most_recent_image, jpeg_image, jpeg_img_size);
            s_most_recent_image_size = jpeg_img_size;
        }

        xSemaphoreGive(s_most_recent_image_mutex);
    }
    else
    {
        ESP_LOGE(TAG, "JPEG image mutex error");
    }
}

// Get the latest JPEG image
// Note: Caller must free the returned pointer after use
uint8_t *get_most_recent_image(size_t *jpeg_img_size)
{
    uint8_t *image_copy = NULL;

    if (s_most_recent_image_mutex == NULL)
    {
        ESP_LOGE(TAG, "JPEG image mutex not initialized");
        return NULL;
    }

    xSemaphoreTake(s_most_recent_image_mutex, portMAX_DELAY);
    if (s_most_recent_image != NULL && s_most_recent_image_size > 0)
    {
        image_copy = malloc(s_most_recent_image_size);
        if (image_copy != NULL)
        {
            memcpy(image_copy, s_most_recent_image, s_most_recent_image_size);
            *jpeg_img_size = s_most_recent_image_size;
        }
    }
    xSemaphoreGive(s_most_recent_image_mutex);

    return image_copy;
}
