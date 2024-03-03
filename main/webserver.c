#include "webserver.h"

/* Handler for the GET request on "/hello" */
esp_err_t hello_get_handler(httpd_req_t *req)
{
    const char* resp_str = "Hello, world!";
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

/* URI handler structure for GET /hello */
httpd_uri_t uri_get = {
    .uri      = "/hello",
    .method   = HTTP_GET,
    .handler  = hello_get_handler,
    .user_ctx = NULL
};

/* Start the web server */
httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    // Start the HTTP server
    if (httpd_start(&server, &config) == ESP_OK) {
        // Register URI handlers
        httpd_register_uri_handler(server, &uri_get);
    }
    return server;
}

/* Stop the web server */
void stop_webserver(httpd_handle_t server)
{
    if (server) {
        // Stop the HTTPD server
        httpd_stop(server);
    }
}
