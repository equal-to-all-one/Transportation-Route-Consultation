#include "Administrator.h"
#include<iostream>
Administrator::Administrator(std::string username, std::string passward): username(username), password(passward)
{
    std::cout << "welcome!" <<  username << std::endl;
}

void Administrator::add_Route(Route_Graph &route_graph)
{
    // Route(std::string start_city, std::string destination_city, std::string start_time, 
    //   std::string arrival_time, float money_cost,std::string time_cost, float distance, Transport_Mode mode);
    std::string start_city, destination_city, start_time, arrival_time, time_cost;
    float money_cost, distance;
    Transport_Mode mode;
    std::string s;
    std::cout << "请输入你想添加路线的起始城市: ";
    std::cin >> std::ws;
    std::getline(std::cin, s);
    start_city = s;
    s.clear();
    std::cout << "请输入你想添加路线的终点城市: ";
    std::cin >> std::ws;
    std::getline(std::cin, s);
    destination_city = s;
    s.clear();
    std::cout << "请输入你想添加路线的起始时间（输入样式:2024-03-21 03:21): ";
    std::cin >> std::ws;
    std::getline(std::cin, s);
    start_time = s;
    s.clear();
    std::cout << "请输入你想添加路线的抵达时间（输入样式:2024-03-21 12:30): ";
    std::cin >> std::ws;
    std::getline(std::cin, s);
    arrival_time = s;
    s.clear();
    std::cout << "请输入你想添加路线的费用(元)（输入样式:300.5 or 250): ";
    std::cin >> std::ws;
    std::cin >> money_cost;
    std::cout << "请输入你想添加路线的花费时间（输入样式: 6 hours 9 minutes): ";
    std::cin >> std::ws;
    std::getline(std::cin, s);
    time_cost = s;
    s.clear();
    std::cout << "请输入你想添加路线的距离(km)（输入样式:100.92 or 120): ";
    std::cin >> std::ws;
    std::cin >> distance;
    std::cout << "请输入你想添加路线的交通方式(输入序号): 1. airplane or 2. high_speed_way : ";
    std::cin >> std::ws;
    int i;
    std::cin >> i;
    if (i != 1 && i != 2) {
        std::cout << "添加失败，请选择正确的交通方式!";
        return;
    }
    mode = i == 1 ? Transport_Mode::airplane : Transport_Mode::high_speed_way;
    Route route(start_city, destination_city, start_time, arrival_time, money_cost, time_cost, distance, mode);
    if (route_graph.add_Route(route)) {
        std::cout << "添加成功" << std::endl;
    }
    else {
        std::cout << "添加失败" << std::endl;
    }
}

void Administrator::delete_Route(Route_Graph &route_graph)
{
    // bool delete_Route(std::string origin, std::string destination, int route_id);
    std::string start_city, destination_city;
    std::string s;
    int id;
    std::cout << "请输入你想删除路线的起始城市: ";
    std::cin >> std::ws;
    std::getline(std::cin, s);
    start_city = s;
    s.clear();
    std::cout << "请输入你想删除路线的终点城市: ";
    std::cin >> std::ws;
    std::getline(std::cin, s);
    destination_city = s;
    s.clear();
    std::cout << "如果你不知道路线id,请输入1,否则输入2继续操作: ";
    int i;
    std::cin >> i;
    if (i == 1) {
        route_graph.consult_City(start_city);
        std::cout << "请输入你想删除路线的id: ";
        std::cin >> std::ws >> id;
    }
    else if (i == 2) {
        std::cout << "请输入你想删除路线的id: ";
        std::cin >> std::ws >> id;
    }
    else {
        std::cout << "无效输入" << std::endl; 
        return;
    }
    if (route_graph.delete_Route(start_city, destination_city, id)) {
        std::cout << "删除成功" << std::endl;
    }
    else {
        std::cout << "删除失败" << std::endl;
    }
}

void Administrator::modify_Route(Route_Graph &route_graph)
{
    // bool Route_Graph::modify_Route(std::string origin, std::string destination, int route_id)
    std::string start_city, destination_city;
    std::string s;
    int id;
    std::cout << "请输入你想修改路线的起始城市: ";
    std::cin >> std::ws;
    std::getline(std::cin, s);
    start_city = s;
    s.clear();
    std::cout << "请输入你想修改路线的终点城市: ";
    std::cin >> std::ws;
    std::getline(std::cin, s);
    destination_city = s;
    s.clear();
    std::cout << "如果你不知道路线id,请输入1,否则输入2继续操作: ";
    std::cin >> std::ws;
    int i;
    std::cin >> i;
    if (i == 1) {
        route_graph.consult_City(start_city);
        std::cout << "请输入你想修改路线的id: ";
        std::cin >> std::ws >> id;
    }
    else if (i == 2) {
        std::cout << "请输入你想修改路线的id: ";
        std::cin >> std::ws >> id;
    }
    else {
        std::cout << "无效输入" << std::endl; 
        return;
    }
    if (route_graph.modify_Route(start_city, destination_city, id)) {
        std::cout << "修改成功" << std::endl;
    }
    else {
        std::cout << "修改失败" << std::endl;
    }
}

void Administrator::consult_Route(Route_Graph &route_graph)
{
    // bool Route_Graph::consult_Route(std::string origin, std::string destination, int route_id)
    std::string start_city, destination_city;
    std::string s;
    int id;
    std::cout << "请输入你想查询路线的起始城市: ";
    std::cin >> std::ws;
    std::getline(std::cin, s);
    start_city = s;
    s.clear();
    std::cout << "请输入你想查询路线的终点城市: ";
    std::cin >> std::ws;
    std::getline(std::cin, s);
    destination_city = s;
    s.clear();
    std::cout << "如果你不知道路线id,请输入1,否则输入2继续操作: ";
    std::cin >> std::ws;
    int i;
    std::cin >> i;
    if (i == 1) {
        route_graph.consult_City(start_city);
        std::cout << "请输入你想查询路线的id: ";
        std::cin >> id;
    }
    else if (i == 2) {
        std::cout << "请输入你想查询路线的id: ";
        std::cin >> id;
    }
    else {
        std::cout << "无效输入" << std::endl; 
        return;
    }
    if (route_graph.consult_Route(start_city, destination_city, id)) {
        std::cout << "查询成功" << std::endl;
    }
    else {
        std::cout << "查询失败" << std::endl;
    }
}

void Administrator::add_City(Route_Graph &route_graph)
{
    // bool Route_Graph::add_City(std::string city_name, double longitude, double altitude)
    std::string city_name; 
    double longitude, altitude;
    std::cout << "请输入你想添加城市的名称: ";
    std::cin >> std::ws;
    std::getline(std::cin, city_name);
    std::cout << "请输入你想添加城市的经度(请输入十进制表示): ";
    std::cin >> std::ws;
    std::cin >> altitude;
    std::cout << "请输入你想添加城市的纬度(请输入十进制表示): ";
    std::cin >> std::ws;
    std::cin >> longitude;
    if(route_graph.add_City(city_name, longitude, altitude)) {
        std::cout << "添加成功" << std::endl;
    }
    else {
        std::cout << "添加失败" << std::endl ;
    }
}

void Administrator::delete_City(Route_Graph &route_graph)
{
    // bool Route_Graph::delete_City(std::string city_name)
    std::string city_name; 
    std::cout << "请输入你想删除城市的名称: ";
    std::cin >> std::ws;
    std::getline(std::cin, city_name);
    if (route_graph.delete_City(city_name)) {
        std::cout << "删除成功" << std::endl;
    }
    else {
        std::cout << "删除失败" << std::endl;
    }
}

void Administrator::consult_City(Route_Graph &route_graph)
{
    std::string city_name; 
    std::cout << "请输入你想查询城市的名称: ";
    std::cin >> std::ws;
    std::getline(std::cin, city_name);
    route_graph.consult_City(city_name);
}

bool Administrator::quit()
{
    std::cout << "你已退出登录" << std::endl;
    return true;
}


