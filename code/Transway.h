#ifndef Transway_h
#define Transway_h

//交通方式类
#include<string>
enum class Transport_Mode {
    none,
    airplane,
    high_speed_way
};

std::string transportModeToString(Transport_Mode mode);
Transport_Mode stringToTransportMode(const std::string& modeStr);
#endif