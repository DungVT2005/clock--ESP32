#include "Buzzer.h"

Buzzer::Buzzer(uint8_t pwmChannel) {
    channel = pwmChannel;
}

void Buzzer::begin() {
    ledcSetup(channel, 1000, 8); // Kênh, Tần số khởi tạo, Độ phân giải
    ledcAttachPin(25, channel);
    stop();
}

void Buzzer::playAlarm() {
    ledcWriteTone(channel, 5000); // Tần số 5kHz cho báo thức
}

void Buzzer::playTimer() {
    ledcWriteTone(channel, 1000); // Tần số 1kHz cho timer
}

void Buzzer::stop() {
    ledcWriteTone(channel, 0);    // Tần số 0 để tắt
}

void Buzzer::beepShort() {
    ledcWriteTone(channel, 2000);
    delay(50);
    ledcWriteTone(channel, 0);
}