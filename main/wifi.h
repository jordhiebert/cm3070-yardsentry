#ifndef WIFI_H
#define WIFI_H

#include "esp_err.h"

#define WIFI_SSID "ZiPgYVwB"
#define WIFI_PASSWORD "d*4$K4@HJxmJ!TNw"

#ifdef __cplusplus
extern "C" {
#endif

// Function declarations
esp_err_t wifi_init_sta(void);

#ifdef __cplusplus
}
#endif

#endif // WIFI_H