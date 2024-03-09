#include "detection_responder.h"
#include "tensorflow/lite/micro/micro_log.h"

#include "esp_main.h"
#include "esp_timer.h"
#include "esp_camera.h"
#include "img_converters.h"
#include "image_store.h"
#include "mqtt.h"
#include "esp_log.h"

#define UPDATE_TIME 1000
#define HOLD_TIME 5000

static int64_t hold_timer = 0;
static int64_t update_timer = 0;
uint8_t *jpeg_image = NULL;
size_t jpeg_img_size = 0;
camera_fb_t *camera_fb;

static const char *TAG = "detection_responder";

void RespondToDetection(float person_score, float no_person_score)
{
    int person_score_int = (person_score) * 100 + 0.5;
    (void)no_person_score; // unused

    if (hold_timer == 0)
    {
        hold_timer = esp_timer_get_time();
    }

    if (update_timer == 0)
    {
        update_timer = esp_timer_get_time();
    }

    if (((esp_timer_get_time() - update_timer) / 1000) >= UPDATE_TIME)
    {
        camera_fb = esp_camera_fb_get();
        free(jpeg_image);
        bool ret = frame2jpg(camera_fb, 80, &jpeg_image, &jpeg_img_size);

        if (ret != true)
        {
            ESP_LOGE(TAG, "JPEG conversion failed");
        }

        save_most_recent_image(jpeg_image, jpeg_img_size);
        esp_camera_fb_return(camera_fb);

        update_timer = 0;
    }

    if (person_score_int > 60)
    {
        if (((esp_timer_get_time() - hold_timer) / 1000) >= HOLD_TIME)
        {
            ESP_LOGI(TAG, "Person detected");

            save_last_detection_image(jpeg_image, jpeg_img_size);
            mqtt_publish("cm3070-yardsentry/detection", "detected");

            hold_timer = 0;
        }
    }
}
