#include "Transway.h"

//将枚举值转为字符串
std::string transportModeToString(Transport_Mode mode)
{
    switch (mode) {
        case Transport_Mode::airplane : return "airplane";
        case Transport_Mode::high_speed_way : return "high_speed_way";
        case Transport_Mode::none: return "none";
        // Add other cases if necessary
        default: return "Unknown";
    }
}

//将字符串转换为枚举值
Transport_Mode stringToTransportMode(const std::string &modeStr)
{
    if (modeStr == "airplane") return Transport_Mode::airplane;
    if (modeStr == "high_speed_way") return Transport_Mode::high_speed_way;
    if (modeStr == "none") return Transport_Mode::none;
    // Add other cases if necessary
    return Transport_Mode::airplane; // Default case
}
