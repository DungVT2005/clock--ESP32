#include "Stopwatch.h"

void Stopwatch::start() { 
    if(!running) { 
        startT = millis() - elapsed; running = true; 
    } 
}
void Stopwatch::stop() { 
    if(running) { 
        elapsed = millis() - startT; running = false; 
    } 
}
void Stopwatch::reset() { 
    elapsed = 0; 
    running = false; 
}
String Stopwatch::getDisplay() {
    uint32_t t = running ? (millis() - startT) : elapsed;
   uint32_t totalSeconds = t / 1000; // Đổi ra giây nguyên bản
    
    uint32_t mins = totalSeconds / 60;
    uint32_t secs = totalSeconds % 60;
    
    char buf[10];
    sprintf(buf, "%02d:%02d", mins, secs);
    return String(buf);
}