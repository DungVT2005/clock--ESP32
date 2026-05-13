#ifndef MENU_H
#define MENU_H
#include "BaoThuc.h"
#include "Timer.h"
#include "Stopwatch.h"

class Menu {
public:
    bool isMenuOpen = false;
    void showMenu();
    // Truyền tham chiếu  vào để Menu trực tiếp cài đặt
    void handleInput(BaoThuc &bt, Timer &tmr, Stopwatch &sw); 
    void printModeInstructions(int mode); // Hàm in hướng dẫn nhập liệu cho từng mode
};
#endif