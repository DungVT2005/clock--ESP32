#ifndef MAXSENSOR_H
#define MAXSENSOR_H
#include "MAX30105.h"

class MaxSensor {
private:
    MAX30105 sensor;
    long lastBeat;
    bool isDetected = false;
    uint32_t lastRetry = 0; //  HẸN GIỜ KẾT NỐI LẠI
    // Thêm các biến để lọc trung bình BPM
    static const byte RATE_SIZE = 4; 
    byte rates[RATE_SIZE]; 
    byte rateSpot = 0;
    float dcFilter_v_prev = 0.0; //  trạng thái của bộ lọc DC
    float lpf_prev = 0.0;        //  trạng thái của bộ lọc thông thấp
    float dynamicThreshold = 0.0;// Ngưỡng dò đỉnh 
public:
    void begin();
    void read();
    String getAlerts();
};
#endif