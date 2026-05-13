#include "Menu.h"
#include "Global.h"

void Menu::showMenu() {
    Serial.println("\n===== MENU LUA CHON =====");
    Serial.println("0: Clock");
    Serial.println("1: Alarm");
    Serial.println("2: Timer");
    Serial.println("3: Stopwatch");
    Serial.println("4: Nhiet do & Do am (DHT22)");
    Serial.println("5: Nhip tim (MAX30102)");
}
void Menu::printModeInstructions(int mode){
    Serial.printf("\n=> DA CHUYEN SANG MODE %d\n", currentMode);
        Serial.println("-------------------------");
        
        // Hiển thị hướng dẫn nhập  cho từng chức năng
        if (currentMode == 1) Serial.println("=> Nhap gio bao thuc (HH:MM), chon c de kiem tra ");
        else if (currentMode == 2) Serial.println("=> Nhap so phut hen gio, ");
        else if (currentMode == 3) Serial.println("=> Nhap 'b' de Bat/Dung, 'r' de Reset");
    
}
void Menu::handleInput(BaoThuc &bt, Timer &tmr, Stopwatch &sw) {
    if (!Serial.available()) return;
    
    // Đọc toàn bộ chuỗi người dùng nhập 
    String input = Serial.readStringUntil('\n');
    input.trim(); // Xóa khoảng trắng thừa
    if (input.length() == 0) return;

    // Tắt chuông (ưu tiên cao nhất)
    if (input == "s" || input == "S") {
        isAlarmActive = false; isTimerActive = false;
        Serial.println("\n[DA TAT CHUONG]");
        return;
    }

    // Mở Menu
    if (input == "m" || input == "M") {
        isMenuOpen = true;
        showMenu();
        return;
    }

    // Chọn Mode từ Menu
    if (isMenuOpen && input.length() == 1 && input[0] >= '0' && input[0] <= '5') {
        currentMode = input[0] - '0';
        isMenuOpen = false;
        printModeInstructions(currentMode);
        return;
    }
    // Xử lý logic nhập liệu khi đang ở trong các Mode tương ứng
    if (!isMenuOpen) {
        if (currentMode == 1) { // Mode Alarm
        if (input == "c" || input == "C") {
        if (bt.isSet()) {
            Serial.println("\n--- KIEM TRA BAO THUC ---");
            Serial.printf("=> Dang bat bao thuc luc: %02d:%02d,an d de xoa\n", bt.getHour(), bt.getMin());
           
        } else {
            Serial.println("\n=>  CHUA CO bao thuc nao .");
        }
        return;
    }
    //  Nhấn 'd' để xóa báo thức
        if (input == "d" || input == "D") {
        bt.set(-1, -1); // Gán giá trị không hợp lệ để vô hiệu hóa
        Serial.println("\n[DA XOA BAO THUC]");
        return;
    }

    // 2. Xử lý nhập HH:MM (Đảm bảo lấy được phút)
        int delim = input.indexOf(':');
        if (delim > 0) {
        String sHour = input.substring(0, delim);
        String sMin = input.substring(delim + 1);
        
        sHour.trim(); // Xóa khoảng trắng/ký tự rác \r \n
        sMin.trim();

        if (sHour.length() > 0 && sMin.length() > 0) {
            int h = sHour.toInt();
            int m = sMin.toInt();

            if (h >= 0 && h < 24 && m >= 0 && m < 60) {
                bt.set(h, m);
                Serial.printf("=> DA DAT BAO THUC LUC %02d:%02d\n", h, m);
            } else {
                Serial.println("=> LOI: Gio (0-23), Phut (0-59)!");
            }
        }
    } else {
        // Nếu nhập sai định dạng hoặc chỉ nhập số mà không có dấu ':'
        Serial.println("=> HUONG DAN: Nhap HH:MM để đặt bao thuc.");
    }
}
        else if (currentMode == 2) { // Setup Timer
            int mins = input.toInt();
            if (mins > 0) {
                tmr.set(mins * 60); // Đổi ra giây
                Serial.printf("=> DA DAT TIMER %d PHUT\n", mins);
            }
        }
        else if (currentMode == 3) { // Control Stopwatch
            if (input == "b" || input == "B") {
                if (sw.isRunning()) sw.stop();
                else sw.start();
            } else if (input == "r" || input == "R") {
                sw.reset();
                Serial.println("=> DA RESET STOPWATCH");
            }
            
        }
    }
}