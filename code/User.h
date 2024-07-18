// 该文件为用户头文件，用于实现用户的相关操作。
#ifndef User_h
#define User_h
#include<string>
#include "Route_Graph.h"
class User
{
    public:
        User();//构造函数
        //查询操作
        void query(Route_Graph& route_graph);
        //退出操作
        bool quit();
        //析构函数
        ~User() = default;
};
#endif