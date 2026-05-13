#ifndef TIMER_H
#define TIMER_H
#include <Arduino.h>

class Timer {
private:
    uint32_t rem, lastTick;
    bool active;
public:
    Timer() : rem(0), active(false) {}
    void set(int sec);
    void update();
    String getDisplay();
    bool isActive() { return active; }
    bool isRunning() { return active; }
};
#endif