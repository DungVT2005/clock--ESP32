#include "DHTSensor.h"
#include "Global.h"

DHTSensor::DHTSensor() : dht(4, DHT22) {}
void DHTSensor::begin() { 
    dht.begin(); 
}
void DHTSensor::read() {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (!isnan(t)) { currentTemp = t; currentHum = h; }
}
String DHTSensor::getAlerts() {
    String s = "";
    if (currentTemp > 36) s += "(!) Nhiet do cao\n";
    if (currentTemp < 12) s += "(!) Nhiet do thap\n";
    if (currentHum > 80) s += "(!) Do am cao\n";
    if (currentHum < 50) s += "(!) Do am thap\n";
    return s;
}