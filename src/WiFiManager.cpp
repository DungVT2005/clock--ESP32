#include "WiFiManager.h"

void WiFiManager::connect(const char* ssid, const char* pass) {
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) { delay(500); }
}
bool WiFiManager::isConnected() { 
    return WiFi.status() == WL_CONNECTED; 
}