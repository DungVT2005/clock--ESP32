#ifndef GLOBAL_H
#define GLOBAL_H
#include <Arduino.h>

#define DHT_PIN 4
#define BUZZER_PIN 25
#define SDA_PIN 21
#define SCL_PIN 22
#define BUTTON_PIN 33 
extern volatile int currentMode; 
extern volatile float currentTemp, currentHum, currentBPM;
extern volatile bool isAlarmActive, isTimerActive;
extern byte uiTick; // Biến đếm nhịp cho UI (tăng mỗi 500ms, reset sau khi in ra máy tính)
extern SemaphoreHandle_t i2cMutex;

#endif