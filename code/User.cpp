#include"User.h"
#include<iostream>
// using namespace std;
User::User(){}
void User::query(Route_Graph& route_graph)
{
    std::string start_city, destination_city, start_time;
    Transport_Mode mode;
    std::string s;
    std::cout << "请输入你想查询路线的起始城市:";
    std::cin >> std::ws;
    // std::cout << std::endl;
    std::getline(std::cin, s);
    start_city = s;
    s.clear();
    std::cout << "请输入你想查询路线的终点城市:";
    std::getline(std::cin, s);
    destination_city = s;
    s.clear();
    std::cout << "请输入出发时间（输入样式:2024-03-21 03:21): ";
    std::getline(std::cin, s);
    start_time = s;
    s.clear();
    std::cout << "请输入你想查询路线的交通方式(输入序号): 1. airplane or 2. high_speed_way: ";
    int i;
    std::cin >> i;
    if (i != 1 && i != 2) {
        std::cout << "添加失败，请选择正确的交通方式!";
        return;
    }
    mode = (i == 1) ? Transport_Mode::airplane : Transport_Mode::high_speed_way;
    int factor;
    std::cout << "请选择你最关心的因素，将为你推荐合适的出行路线（输入序号）: 1. 距离 2. 费用 3.时间 : ";
    std::cin >> factor;
    if (factor == 1) {
        route_graph.calculate_Fit_Distance(start_city, destination_city, mode, start_time);
    }
    else if (factor == 2) {
        route_graph.calculate_Fit_Cost(start_city, destination_city, mode, start_time);
    }
    else if (factor == 3) {
        route_graph.calculate_Fit_Time(start_city, destination_city, mode, start_time);
    }
    else {
        std::cout << "错误输入" << std::endl;
    }
}

bool User::quit()
{    
    std::cout << "你已退出";
    return true;
}
