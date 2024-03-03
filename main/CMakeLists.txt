idf_component_register(
    SRCS
        "detection_responder.cc"
        "image_provider.cc"
        "main.cc"
        "tf_main_functions.cc"
        "model_settings.cc"
        "person_detect_model_data.cc"
        "app_camera_esp.c"
        "wifi.c"
        "webserver.c"

    PRIV_REQUIRES
        spi_flash
        esp_wifi
        nvs_flash
        esp_http_server
        esp_netif

    INCLUDE_DIRS "")
