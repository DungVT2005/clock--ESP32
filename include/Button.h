#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
private:
    uint8_t pin;
    uint32_t pressStartTime = 0;
    bool isLongPressHandled = false;

public:
    // Hàm khởi tạo
    Button(uint8_t p); 
    // Hàm cấu hình chân 
    void begin();  
    // Hàm kiểm tra trạng thái    
    int check();       
};

#endif