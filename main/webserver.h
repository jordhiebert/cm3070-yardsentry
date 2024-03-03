#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "esp_http_server.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function declarations
httpd_handle_t start_webserver(void);
void stop_webserver(httpd_handle_t server);

#ifdef __cplusplus
}
#endif

#endif // WEBSERVER_H
