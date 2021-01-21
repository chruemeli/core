#include "HTTPRequest.h"

HTTPRequest::HTTPRequest() {
    Logger::info(F("HTTPC"), F("Successfully initialized"));
}

void HTTPRequest::begin(String url) {
    this->httpClient = new HTTPClient();
    this->wifiClient = new WiFiClient();

    this->httpClient->begin(*this->wifiClient, url);
}

int HTTPRequest::GET() {
    return this->httpClient->GET();
}

int HTTPRequest::GET(String url) {
    this->begin(url);
    return this->httpClient->GET();
}

uint8_t HTTPRequest::read() {
    return this->wifiClient->read();
}

String HTTPRequest::readString() {
    return this->wifiClient->readString();
}

void HTTPRequest::setHeader(String name, String value) {
    this->httpClient->addHeader(name, value);
}

WiFiClient* HTTPRequest::getClient() {
    return this->wifiClient;
}

void HTTPRequest::close() {
    delete this->httpClient;
    delete this->wifiClient;
}