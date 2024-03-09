#ifndef MQTT_H
#define MQTT_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_BROKER_URI "mqtt://192.168.11.2"
#define MQTT_CLIENT_ID "cm3070-yardsentry"
#define MQTT_USERNAME "cm3070-yardsentry"
#define MQTT_PASSWORD "aja!CZR4ymk7vrk2ufz"

// Initializes the MQTT client and connects it to the broker
esp_err_t mqtt_init(void);

// Publishes a message to a specified topic
esp_err_t mqtt_publish(const char* topic, const char* message);

#ifdef __cplusplus
}
#endif

#endif // MQTT_H
