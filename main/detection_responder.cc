#include "detection_responder.h"
#include "tensorflow/lite/micro/micro_log.h"

#include "esp_main.h"
#include "esp_timer.h"
#include "esp_camera.h"
#include "img_converters.h"
#include "image_store.h"
#include "mqtt.h"
#include "esp_log.h"

#define HOLD_TIME 5000

static int64_t elapsed_time = 0;
uint8_t *jpeg_image = NULL;
size_t jpeg_img_size = 0;
camera_fb_t *camera_fb;

static const char *TAG = "detection_responder";

void RespondToDetection(float person_score, float no_person_score)
{
    int person_score_int = (person_score) * 100 + 0.5;
    (void)no_person_score; // unused

    if (elapsed_time == 0)
    {
        elapsed_time = esp_timer_get_time();
    }

    if (person_score_int > 60)
    {
        if (((esp_timer_get_time() - elapsed_time) / 1000) >= HOLD_TIME)
        {
            camera_fb = esp_camera_fb_get();
            ESP_LOGI(TAG, "Person detected");
            free(jpeg_image);
            bool ret = frame2jpg(camera_fb, 80, &jpeg_image, &jpeg_img_size);
            if (ret != true)
            {
                ESP_LOGE(TAG, "JPEG conversion failed");
            }

            save_jpeg_image(jpeg_image, jpeg_img_size);
            mqtt_publish("cm3070-yardsentry/detection", "detected");

            esp_camera_fb_return(camera_fb);
            elapsed_time = 0;
        }
    }
}
