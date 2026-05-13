#include "BaoThuc.h"
#include "Global.h"

void BaoThuc::set(int h, int m) {
    alarmHour = h;
    alarmMin = m;
    enabled = (h != -1); 
    isDismissed = false; // Reset cờ khi đặt báo thức mới
    saveToFlash();
}
void BaoThuc::clear(){
    alarmHour = -1;
    alarmMin = -1;
    enabled = false;
    isDismissed = false; // Reset cờ khi xóa báo thức
    saveToFlash();
}
// ham luu bao thuc vao flash
void BaoThuc::saveToFlash() {
    prefs.begin("alarm-db", false); // Mở thư mục alarm-db
    prefs.putInt("h", alarmHour);
    prefs.putInt("m", alarmMin);
    prefs.putBool("active", enabled);
    prefs.end();
}
void BaoThuc::loadFromFlash() {
    prefs.begin("alarm-db", true); // Mở thư mục alarm-db ở chế độ chỉ đọc (true)
    alarmHour = prefs.getInt("h", -1);
    alarmMin = prefs.getInt("m", -1);
    enabled = prefs.getBool("active", false);
    prefs.end();
}
void BaoThuc::check(int h, int m, int s) {
    if (!enabled || alarmHour == -1) return; // Nếu đã xóa thì thoát luôn
    
    if (h == alarmHour && m == alarmMin && s == 0) {
        isAlarmActive = true;
        isDismissed = false;
    }
    if (h == alarmHour && m == alarmMin) {
    if (isDismissed) {
        isAlarmActive = false; 
    }
}
if (m != alarmMin) {
        isDismissed = false;
    }
}
void BaoThuc::silence() {
    isAlarmActive = false;
    isDismissed = true; // Đánh dấu đã tắt thủ công
}