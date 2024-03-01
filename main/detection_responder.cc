/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

/*
 * SPDX-FileCopyrightText: 2019-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "detection_responder.h"
#include "tensorflow/lite/micro/micro_log.h"

#include "esp_main.h"
#include "esp_timer.h"
#include "esp_camera.h"
#include "img_converters.h"

#define HOLD_TIME 5000

static int64_t elapsed_time = 0;
uint8_t *jpeg_image = NULL;
size_t jpeg_img_size = 0;
camera_fb_t *camera_fb;

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
            MicroPrintf("TRIGGER!");
            free(jpeg_image);
            bool ret = frame2jpg(camera_fb, 80, &jpeg_image, &jpeg_img_size);
            if (ret != true)
            {
                MicroPrintf("JPEG conversion failed");
            }
            //esp_err_t esp_ret = smtp_client_send_email(jpeg_image, jpeg_img_size);
            //if (esp_ret != ESP_OK)
            //{
            //    ESP_LOGE(TAG, "Failed to send the email, returned %02X", esp_ret);
            //}
            esp_camera_fb_return(camera_fb);
            elapsed_time = 0;
        }
    }
    else
    {
        //MicroPrintf("No person detected!");
    }
}
