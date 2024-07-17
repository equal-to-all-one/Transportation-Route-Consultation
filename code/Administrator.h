#ifndef Administrator_h
#define Administrator_h
#include<string>
#include "Route_Graph.h"
class Administrator
{
    private:
        std::string username;
        std::string password;
    public:
        Administrator(std::string username, std::string password);

        //管理员可以执行的对路线的增删改查操作
        void add_Route(Route_Graph& route_graph);
        void delete_Route(Route_Graph& route_graph);
        void modify_Route(Route_Graph& route_graph);
        void consult_Route(Route_Graph& route_graph);

        //管理员可以执行的对城市的增删改查操作
        void add_City(Route_Graph& route_graph);
        void delete_City(Route_Graph& route_graph);
        void consult_City(Route_Graph& route_graph);

        bool quit();
};
#endif