#ifndef CLOCK_H
#define CLOCK_H
#include <Arduino.h>
#include <time.h>

class Clock {
public:
    void syncNTP();
    String getTimeStr();
    void getHM(int &h, int &m, int &s);
    String getDateStr();
};
#endif