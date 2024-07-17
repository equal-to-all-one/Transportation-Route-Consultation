//该文件用于实现一些与实现相关的函数
#ifndef Other_h
#define Other_h

#include "Route_Graph.h"
#include "User.h"
#include "Administrator.h"
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip> //get_time
#include<ctime>
#include<chrono>

//以下函数用于计算中转时间
//需要注意，在这里，我们只考虑了中转时间小于24小时的情况。

//解析字符串函数
//这里没有异常处理操作，需要严格按照格式输入
std::tm parse_Time(const std::string& time);

//计算中转时间
std::pair<int,int> calculate_Transfer_Time(const std::string& arrival_time_str, const std::string& start_time_str);
//计算花费时间
long long calculate_Time_In_Minutes(const std::string& time_str);

//以下函数用于管理员登录
//"administrator_info.txt"是默认的储存账号信息的文件
bool read_Administrator_Infomation(std::string& administrator_name, std::string& password, std::string filename = "administrator_info.txt");

bool authenticate(const std::string& input_Administrator_Name,  const std::string& input_Password);


// 主界面显示
void display_Main_Menu();


//管理员界面显示
void display_Admin_Menu();

//用户界面显示
void display_User_Menu();

//用户操作界面
void user_Interface(Route_Graph& route_graph);

//管理员操作界面
void admin_Interface(Route_Graph& route_graph);

#endif