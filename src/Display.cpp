#include "Display.h"

void Display::printData(int mode, String time, String content, String alerts) {
    // Nếu có cảnh báo thì in cảnh báo
    if (alerts != "") {
        Serial.print("[CANH BAO] ");
        Serial.println(alerts);
    }
    
    // Xử lý in theo Mode
    if (mode == 0) {
        // Mode 0 chỉ in ra giờ
        Serial.println(time);
    } else {
        // Các mode khác in ra: Giờ | Dữ liệu mode
        Serial.print(time);
        Serial.print(" | ");
        Serial.println(content);
    }
}