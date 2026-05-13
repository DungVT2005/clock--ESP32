#include "Buzzer.h"

Buzzer::Buzzer(uint8_t pwmChannel) {
    channel = pwmChannel;
}

void Buzzer::begin() {
    ledcSetup(channel, 2000, 8); // Kênh, Tần số khởi tạo, Độ phân giải
    ledcAttachPin(25, channel);
    stop();
}

void Buzzer::playAlarm() {
    int melody[] = { 2637, 2093, 2637, 3136 }; 
    int durations[] = { 150, 150, 150, 300 };
    int numNotes = sizeof(melody) / sizeof(melody[0]);
    for (int i = 0; i < numNotes; i++) {
        if (!isAlarmActive) { 
            stop();
            return; 
        }
        if (melody[i] == 0) {
            stop();
        } else {
            ledcWriteTone(channel, melody[i]); // Phát nốt nhạc
        }
        vTaskDelay(pdMS_TO_TICKS(durations[i])); 
    }
    stop();
}
void Buzzer::playTimer() {
    ledcWriteTone(channel, 1000); // Tần số 1kHz cho timer
}

void Buzzer::stop() {
    ledcWriteTone(channel, 0);    // Tần số 0 để tắt
    ledcWrite(channel, 0);
}

void Buzzer::beepShort() {
    ledcWriteTone(channel, 2000);
    vTaskDelay(pdMS_TO_TICKS(50));
    ledcWriteTone(channel, 0);
}