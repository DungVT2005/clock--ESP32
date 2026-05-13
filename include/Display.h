#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>

class Display {
public:
    void printData(int mode, String time, String content, String alerts);
};
#endif