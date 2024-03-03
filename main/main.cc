#include <stdio.h>
#include "tf_main_functions.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_main.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "webserver.h"
#include "esp_netif.h"
#include "wifi.h"
#include "image_store.h"

extern "C" void app_main()
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize the TCP/IP stack
    ESP_ERROR_CHECK(esp_netif_init());

    // Create the default event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Initialize and connect WiFi
    ESP_ERROR_CHECK(wifi_init_sta());

    // set up our image store
    image_store_init();

    // Start the web server
    httpd_handle_t server = start_webserver();

    xTaskCreate((TaskFunction_t)&tf_main, "tf_main", 4 * 1024, NULL, 8, NULL);
    vTaskDelete(NULL);
}
