    #include "Clock.h"

    void Clock::syncNTP() {
        configTime(7 * 3600, 0, "pool.ntp.org");
    }   
    String Clock::getTimeStr() {
        struct tm info;
        if(!getLocalTime(&info)) return "Syncing...";
        char buf[10];
        strftime(buf, sizeof(buf), "%H:%M:%S", &info);
        return String(buf);
    }
    void Clock::getHM(int &h, int &m, int &s) {
        struct tm info;
        if(getLocalTime(&info)) { h = info.tm_hour; m = info.tm_min; s = info.tm_sec; }
    }
    String Clock::getDateStr() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return "No Date";
    char buffer[16];
    strftime(buffer, sizeof(buffer), "%a/%d/%b", &timeinfo);
    return String(buffer);
}