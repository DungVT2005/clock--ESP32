#include <Arduino.h>
#include "Global.h"
#include "WiFiManager.h"
#include "Clock.h"
#include "DHTSensor.h"
#include "MaxSensor.h"
#include "BaoThuc.h"
#include "Timer.h"
#include "Stopwatch.h"
#include "Menu.h"
#include "Buzzer.h"
#include "Button.h"
#include <LiquidCrystal_I2C.h>
WiFiManager wifi; Clock clk; DHTSensor dht; MaxSensor pulse;
BaoThuc bt; Timer tmr; Stopwatch sw; Menu menu; Buzzer bz(0);Button btn(33);
LiquidCrystal_I2C lcd(0x27, 16, 2);
void Task_Sensor(void *p) {
    uint32_t lastDHTRead = 0;
    for(;;) {
        // 1. MAX30102 đọc liên tục ngầm ở background
        pulse.read();
        // 2. DHT22 đọc mỗi 2 giây
        if (millis() - lastDHTRead >= 2000) {
            dht.read();
            lastDHTRead = millis();
        }
        vTaskDelay(pdMS_TO_TICKS(20)); 
    }
}
void Task_Logic(void *p) {
    bz.begin();
    for(;;) {
        tmr.update();
        int h, m, s;
        clk.getHM(h, m, s);
        bt.check(h, m, s);
        if (isAlarmActive) bz.playAlarm();
        else if (isTimerActive) bz.playTimer();
        else bz.stop();

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
void Task_UI(void *p) {
    uint32_t lastPrint = 0;
    byte uiTick = 0;
    for(;;) {
        // xử lý phím bấm 
        menu.handleInput(bt, tmr, sw); 
        int btnEvent = btn.check();
        if (btnEvent == 1) { 
            if (isAlarmActive || isTimerActive) {
                isAlarmActive = false; 
                isTimerActive = false;
                Serial.println("\n DA TAT CHUONG ");
            }
        } 
        else if (btnEvent == 2) { 
            currentMode = (currentMode + 1) % 6; 
            menu.printModeInstructions(currentMode);
            
            vTaskDelay(pdMS_TO_TICKS(420)); // Delay để người dùng nhả tay
        }
        if (!menu.isMenuOpen && millis() - lastPrint >= 500) {
            lastPrint = millis();
            // Lấy thông báo cảnh báo 
            String alerts = "";
            if (currentMode == 4) alerts = dht.getAlerts();
            if (currentMode == 5) alerts = pulse.getAlerts();
            uiTick++;
            // IN LÊN MÀN HÌNH LCD 
            if (xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(50)) == pdTRUE) {
                switch(currentMode) {
                    case 0: 
                        lcd.setCursor(0, 0); lcd.printf("%-16s", clk.getTimeStr().c_str());
                        lcd.setCursor(0, 1); lcd.printf("%-13s", clk.getDateStr().c_str()); 
                        break;
                    case 1: 
                        lcd.setCursor(0, 0); 
                        if (bt.isSet()) {
                            lcd.print("ALARM: Dang cho ");
                            lcd.setCursor(0, 1); lcd.print("den gio...      ");
                        } else {
                            lcd.print("ALARM:          ");
                            lcd.setCursor(0, 1); lcd.print("                ");
                        }
                        break;
                    case 2:
                        lcd.setCursor(0, 0); 
                        if (tmr.isRunning()) lcd.printf("TIMER: %-9s", tmr.getDisplay().c_str());
                        else lcd.print("TIMER:          ");
                        lcd.setCursor(0, 1); lcd.print("                ");
                        break;
                    case 3:
                        lcd.setCursor(0, 0); 
                        if (sw.isRunning() || sw.getElapsed() > 0) {
                            lcd.print("STOPWATCH:      ");
                            lcd.setCursor(0, 1); lcd.printf("%-16s", sw.getDisplay().c_str());
                        } else {
                            lcd.print("STOPWATCH:      ");
                            lcd.setCursor(0, 1); lcd.print("                ");
                        }
                        break;
                    case 4:
    
                        lcd.setCursor(0, 0); lcd.printf("T:%.1fC H:%.1f%%  ", currentTemp, currentHum);
                        lcd.setCursor(0, 1); 
                        if (alerts != "") lcd.printf("%-16s", alerts.c_str());
                        else lcd.print("                ");
                        break;
                    case 5:
                        lcd.setCursor(0, 0); lcd.printf("BPM: %-11.1f", currentBPM);

                        lcd.setCursor(0, 1); 
                        if (alerts != "") lcd.printf("%-16s", alerts.c_str());
                        else lcd.print("                ");
                        break;
                }
                xSemaphoreGive(i2cMutex); // TRẢ LẠI CHÌA KHÓA I2C
            }

            //  IN RA serial
            if (uiTick >= 2) {
            if (alerts != "") { Serial.print("[CANH BAO] "); Serial.println(alerts); }
            switch(currentMode) {
                case 0:  
                        Serial.println(clk.getTimeStr()); 
                        Serial.println(clk.getDateStr());
                    break;
                case 1: 
                    if (bt.isSet()) Serial.println("ALARM: Dang cho den gio...");
                    break;
                case 2:
                    if (tmr.isRunning()) Serial.println("TIMER: " + tmr.getDisplay());
                    break;
                case 3:
                    if (sw.isRunning() || sw.getElapsed() > 0) Serial.println("STOPWATCH: " + sw.getDisplay());
                    break;
                case 4:
                    Serial.printf("T: %.1fC, H: %.1f%%\n", currentTemp, currentHum);
                    break;
                case 5:
                    Serial.printf("BPM: %.1f\n", currentBPM);
                    break;
            }
            uiTick=0;
        }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
void setup() {
    Serial.begin(115200);
    btn.begin();
    Wire.begin(21, 22,100000);
    i2cMutex = xSemaphoreCreateMutex();
    lcd.init();       // Khởi tạo LCD
    lcd.backlight();  // Bật đèn nền
    lcd.setCursor(0, 0);
    lcd.print("DONG HO ");
    lcd.setCursor(0, 1);
    lcd.print(" DANG KHOI DONG");
    delay(1500);      
    lcd.clear();
    wifi.connect("Soc", "soc102012");
    clk.syncNTP();
    dht.begin();
    pulse.begin();
    bt.loadFromFlash();
    Serial.println("\n HE THONG DA KHOI DONG");
    //  Đẩy TẤT CẢ sang Core 1. Core 0 chỉ để chạy WiFi nội bộ của ESP32.
    xTaskCreatePinnedToCore(Task_Sensor, "Sensor", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(Task_Logic,  "Logic",  4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(Task_UI,     "UI",     4096, NULL, 1, NULL, 1);
}
void loop() { 
vTaskDelete(NULL); 
}