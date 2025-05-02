#include "ChronoRailSystem.hpp"
#include <iostream>

int main() {
    std::cout << "欢迎使用 ChronoRail 时空列车调度系统！\n";
    std::cout << "输入命令开始操作（输入 Ctrl+D 退出）\n\n";

    ChronoRailSystem system;
    system.run();

    return 0;
} 
