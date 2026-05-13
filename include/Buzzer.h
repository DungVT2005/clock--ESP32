#ifndef BUZZER_H
#define BUZZER_H
#include "Global.h"

class Buzzer {
private:
    uint8_t channel;
public:
    Buzzer(uint8_t pwmChannel = 0);
    void begin();
    void playAlarm();    // Tiếng kêu cho báo thức (cao độ cao)
    void playTimer();    // Tiếng kêu cho timer (cao độ thấp hơn)
    void stop();         // Tắt tiếng
    void beepShort();    // Tiếng bíp ngắn khi bấm nút (phản hồi người dùng)
};

#endif