#ifndef ESP_SIMPLE_HTTP_CLIENT_H_
#define ESP_SIMPLE_HTTP_CLIENT_H_

#include <Arduino.h>
#include <WiFiClient.h>

class ESPSimpleHttpClient {
  public:
    typedef enum {
      ERROR_NONE = 0,
      ERROR_CONNECTION_REFUSED = 1,
      ERROR_SEND_FAILED = 2,
      ERROR_PARSE_HEADER_FAILED = 3,
      ERROR_PARSE_BODY_FAILED = 4,
    } error_code_t;

    error_code_t error;

    void begin(String url, String user="", String password="");

    void end();

    void addHeader(String key, String value);

    bool request(String method, String content="");

    bool get();

    bool put(String content);

    bool body(char* body, size_t length);

    Stream& bodyStream();

  protected:
    WiFiClient client;
    String host;
    uint16_t port = 80;
    String path;
    String auth;
    String headers;

    bool sendRequest(String method, String content);
    bool parseResponseHeaders();
    bool parseResponseBody(char* body, size_t length);
};

#endif // ESP_SIMPLE_HTTP_CLIENT_H_
