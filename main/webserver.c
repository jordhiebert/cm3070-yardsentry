#include "webserver.h"
#include "esp_log.h"
#include "image_store.h"

static const char *TAG = "webserver";

// Handler for the GET request on "/hello"
esp_err_t hello_handler(httpd_req_t *req)
{
    const char *resp_str = "Hello, world!";
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// Handler to serve the JPEG image
esp_err_t last_img_handler(httpd_req_t *req)
{
    size_t img_size;
    uint8_t *img = get_jpeg_image(&img_size);

    if (img == NULL || img_size == 0)
    {
        ESP_LOGE(TAG, "Failed to get JPEG image.");
        httpd_resp_send_404(req);
        return ESP_FAIL;
    }

    httpd_resp_set_type(req, "image/jpeg");
    httpd_resp_send(req, (const char *)img, img_size);

    free(img); // Important: Free the image memory after sending
    return ESP_OK;
}

// URI handler structure for GET /hello
httpd_uri_t uri_hello = {
    .uri = "/hello",
    .method = HTTP_GET,
    .handler = hello_handler,
    .user_ctx = NULL};

// URI handler structure for GET /last_img
httpd_uri_t uri_last_img = {
    .uri = "/last_img",
    .method = HTTP_GET,
    .handler = last_img_handler,
    .user_ctx = NULL};

// Start the web server
httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    // Start the HTTP server
    if (httpd_start(&server, &config) == ESP_OK)
    {
        // Register URI handlers
        httpd_register_uri_handler(server, &uri_hello);
        httpd_register_uri_handler(server, &uri_last_img);
    }
    return server;
}

// Stop the web server
void stop_webserver(httpd_handle_t server)
{
    if (server)
    {
        // Stop the HTTPD server
        httpd_stop(server);
    }
}
