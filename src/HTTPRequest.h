#ifndef httpRequest_h
#define httpRequest_h

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "Logger.h"

class HTTPRequest {
    private:
        HTTPClient *httpClient;
        WiFiClient *wifiClient;
    public:
        HTTPRequest();

        void setHeader(String name, String value);

        void begin(String url);
        WiFiClient* getClient();
        
        int GET();
        int GET(String url);

        uint8_t read();
        String readString();

        void close();
};

#endif