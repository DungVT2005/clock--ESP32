#include "Global.h"
volatile int currentMode = 0;
volatile float currentTemp = 0, currentHum = 0, currentBPM = 0;
volatile bool isAlarmActive = false, isTimerActive = false;
SemaphoreHandle_t i2cMutex = NULL;