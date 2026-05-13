
#ifndef BAOTHUC_H
#define BAOTHUC_H
#include <Preferences.h> // Thư viện để lưu trữ dữ liệu vào flash của ESP32
class BaoThuc {
private:
    int alarmHour = -1; // Khởi tạo bằng -1
    int alarmMin = -1;
    int alarmSec = -1;
    bool enabled = false;
    bool isDismissed = false; // đánh dấu đã tắt chuông
    Preferences prefs; // Đối tượng để quản lý flash
public:
    void set(int h, int m);
    void clear();
    void check(int h, int m,int s);
    int getHour() { return alarmHour; }
    int getMin() { return alarmMin; }
    void silence();
    bool isSet() { return (alarmHour != -1); } // Trả về true nếu đã đặt
    // Hai hàm   quản lý bộ nhớ
    void saveToFlash();
    void loadFromFlash();
};
#endif