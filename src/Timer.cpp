#include "Timer.h"
#include "Global.h"

void Timer::set(int sec) { 
    rem = sec; 
    active = true; 
    lastTick = millis(); 
    isTimerActive = false; 
}
void Timer::update() {
    if (active && rem > 0 && millis() - lastTick >= 1000) {
        rem--; lastTick = millis();
        if (rem == 0) { active = false; isTimerActive = true; }
    }
}
String Timer::getDisplay() {
    char buf[10]; sprintf(buf, "%02d:%02d", rem/60, rem%60);
    return String(buf);
}