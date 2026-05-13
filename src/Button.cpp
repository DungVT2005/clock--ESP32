#include "Button.h"

// Gán chân khi khởi tạo
Button::Button(uint8_t p) {
    pin = p;
}

// Bật điện trở kéo lên
void Button::begin() {
    pinMode(pin, INPUT_PULLUP);
}

// Hàm kiểm tra được gọi liên tục trong vòng lặp UI
int Button::check() {
    bool isPressed = (digitalRead(pin) == LOW);
    int result = 0; // Mặc định là 0 (Không có sự kiện)

    if (isPressed) {
        // Bắt đầu bấm
        if (pressStartTime == 0) {
            pressStartTime = millis();
            isLongPressHandled = false;
        }
        // Đang giữ và đủ 3 giây (3000ms)
        if (!isLongPressHandled && (millis() - pressStartTime > 3000)) {
            isLongPressHandled = true;
            return 2; // Báo hiệu: ĐÃ NHẤN GIỮ
        }
    } else {
        // Vừa buông tay ra
        if (pressStartTime > 0) {
            uint32_t pressDuration = millis() - pressStartTime;

            // Nếu buông tay trước 1 giây và chưa bị dính cờ Long Press
            if (pressDuration < 1000 && !isLongPressHandled) {
                result = 1; // Báo hiệu: ĐÃ NHẤN NGẮN
            }
            pressStartTime = 0; // Reset lại đồng hồ bấm giờ của nút
        }
    }
    
    return result;
}