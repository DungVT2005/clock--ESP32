#ifndef STOPWATCH_H
#define STOPWATCH_H
#include <Arduino.h>

class Stopwatch {
private:
    uint32_t startT, elapsed;
    bool running;
public:
    Stopwatch() : elapsed(0), running(false) {}
    void start();
    void stop();
    void reset();
    String getDisplay();
    bool isRunning() { return running; }
    uint32_t getElapsed() { return elapsed; }
};
#endif
