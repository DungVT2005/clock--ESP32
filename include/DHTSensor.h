#ifndef DHTSENSOR_H
#define DHTSENSOR_H
#include <DHT.h>

class DHTSensor {
private:
    DHT dht;
public:
    DHTSensor();
    void begin();
    void read();
    String getAlerts();
};
#endif