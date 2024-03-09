#include "mqtt.h"
#include "mqtt_client.h"
#include "esp_log.h"

static const char *TAG = "MQTT";
static esp_mqtt_client_handle_t mqtt_client = NULL;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;

    switch (event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;
        // Handle other events as needed
    }
}

esp_err_t mqtt_init(void)
{
    const esp_mqtt_client_config_t mqtt_cfg =
    {
        .broker.address.uri = MQTT_BROKER_URI,
        .credentials =
        {
            .client_id = MQTT_CLIENT_ID,
            .username = MQTT_USERNAME,
            .authentication.password = MQTT_PASSWORD
        }
    };

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    if (mqtt_client == NULL)
    {
        ESP_LOGE(TAG, "Failed to initialize MQTT client");
        return ESP_FAIL;
    }

    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_err_t start_ret = esp_mqtt_client_start(mqtt_client);
    if (start_ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to start MQTT client");
        return start_ret;
    }

    mqtt_publish("cm3070-yardsentry/status", "Online");

    return ESP_OK;
}

esp_err_t mqtt_publish(const char *topic, const char *message)
{
    if (mqtt_client == NULL)
    {
        ESP_LOGE(TAG, "MQTT client not initialized");
        return ESP_FAIL;
    }

    int msg_id = esp_mqtt_client_publish(mqtt_client, topic, message, 0, 1, 0);
    if (msg_id < 0)
    {
        ESP_LOGE(TAG, "Failed to publish message");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Published, msg_id=%d", msg_id);
    return ESP_OK;
}
