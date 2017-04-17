#include "ESPSimpleHttpClient.h"
#include <base64.h>

void ESPSimpleHttpClient::begin(String url, String user, String password) {
  int index = url.indexOf(':');
  url.remove(0, (index + 3));

  index = url.indexOf('/');
  host = url.substring(0, index);
  url.remove(0, index);

  index = host.indexOf(':');
  if(index >= 0) {
    port = host.substring(index + 1).toInt();
    host.remove(index);
  }

  if (user.length() != 0) {
    auth = base64::encode(user + ":" + password);
  }

  path = url;
}

void ESPSimpleHttpClient::end() {
  while (client.connected()) {
    if (client.available()) {
      client.read();
    }
  }
}

void ESPSimpleHttpClient::addHeader(String key, String value) {
  headers += key + ": " + value + "\r\n";
}

bool ESPSimpleHttpClient::request(String method, String content) {
  error = ERROR_NONE;

  if (!client.connect(host.c_str(), port)) {
    error = ERROR_CONNECTION_REFUSED;

    return false;
  }

  if (!sendRequest(method, content)) {
    return false;
  }

  if (!parseResponseHeaders()) {
    return false;
  }

  return true;
}

bool ESPSimpleHttpClient::get() {
  return request("GET");
}

bool ESPSimpleHttpClient::put(String content) {
  return request("PUT", content);
}

bool ESPSimpleHttpClient::body(char* body, size_t length) {
  char* current = body;
  char* end = body + length;

  while (client.connected()) {
    if (current >= end) {
      error = ERROR_PARSE_BODY_FAILED;
      return false;
    }

    if (client.available()) {
      *current = client.read();
      current++;
    }
  }

  *current = 0;

  return true;
}

Stream& ESPSimpleHttpClient::bodyStream() {
  return client;
}

bool ESPSimpleHttpClient::sendRequest(String method, String content) {
  client.print(
    method + " " + path + " HTTP/1.0\r\n" + // request line
    "Host: " + (port != 80 ? host + ":" + port : host) + "\r\n" + // host
    (content.length() != 0 ? (String("Content-Length: ") + content.length() + "\r\n") : "") + // content length
    (auth.length() != 0 ? (String("Authorization: Basic ") + auth + "\r\n") : "") + // authorization
    headers + // custom headers
    "\r\n");

  if (content.length() != 0) {
    client.print(content);
  }

  return true;
}

bool ESPSimpleHttpClient::parseResponseHeaders() {
  while(client.connected()) {
    String line = client.readStringUntil('\n');

    line.trim();

    if (line.length() == 0) {
      return true;
    }
  }

  error = ERROR_PARSE_HEADER_FAILED;

  return false;
}
