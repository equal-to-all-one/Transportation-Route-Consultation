#ifndef Route_Graph_h
#define Route_Graph_h
#include<vector>
#include<queue>
#include <string>
#include<list>
#include"Transway.h"

struct Route;

//顶点类，用于储存城市信息
struct City
{
    City(std::string name, double longitude, double altitude);
    City(const City& City) = default;
    ~City() = default;

    void display_City_Information();//输出城市顶点信息
    void display_City_Route_Table();//输出城市时刻表

    std::string name;//city name
    //the longitude and altitude is used to record the city's position
    double longitude;
    double altitude;

    int city_id;//used to identify city
    static int next_city_id; //类静态成员，用来创建城市的标识
    static int next_route_id;//类静态成员，用来创建城市的标识

    // 城市路线时刻表
    // 无关信息，用于构建邻接表,其中next_route用于构建指向不同城市的链表，而brother_route用于构建两相同城市间的多条路径的链表
    // 本质是广义表或树的兄弟链储存结构的运用
    std::list<std::list<Route>> route_table;
};

//边类，用于储存路线信息
struct Route
{
    //省略了复制构造函数和析构函数的显式声明，也许会出现意想不到的问题
    Route(std::string start_city, std::string destination_city, std::string start_time, 
          std::string arrival_time, float money_cost,std::string time_cost, float distance, Transport_Mode mode);
    Route();
    Route(const Route& Route);
    void display();//输出路线信息
    void set_id(int id);

    int id;//用于标识路线（主键）
    std::string start_city;//起点城市
    std::string destination_city;//终点城市
    std::string start_time;//出发时间
    std::string arrival_time;//抵达时间
    std::string time_cost;//通过起止时间能够计算出花费时间
    float money_cost;//费用
    float distance;//路线长度
    Transport_Mode mode;//出行方式
};

class Route_Graph
{
    private:
        std::vector<City> Adjoin_Graph;//头结点数组，用来储存城市顶点数据
        int Route_Count;
        int City_Count;

    public:
        //初始化操作
        Route_Graph();
        Route_Graph(const Route_Graph& Route_Graph) = default;
        bool initial_route_import(std::string& filename);

        //对路线的增删改查操作
        //std::string origin, std::string destination, int start_time, std::string arrival_time, int time_cost, int money_cost,
        bool add_Route(Route& route);
        bool delete_Route(std::string& origin, std::string& destination, int& route_id);
        bool modify_Route(std::string& origin, std::string& destination, int& route_id);
        bool consult_Route(std::string& origin, std::string& destination, int& route_id);

        // 对城市的增删改查操作
        bool add_City(std::string& city_name, double& longitude, double& altitude);
        bool delete_City(std::string& city_name);
        void consult_City(std::string& city_name);//用于输出城市的路线表，id等信息。
        
        //伪最短路径计算操作(dijkstra)
        //为了符合实际情况，在这里我修改了功能实现目标，以下函数用于实现根据用户首要考虑因素进行合适的路线推荐
        //原因：题目本身要求我们实现最短路径计算，但是没有考虑到在实际路线中，由于不同的城市之间不只有一条路线，而是有多条，从数据结构上而言，
        //我们应该使用的是有向图中的重复边图而不是无重复有向图。再者，在实际过程中，由于哪怕是起点城市和目标城市相同的不同路线，也会由于各种原因使得其
        //出发时间，抵达时间，花费等等不相同，而在题目中，其忽略了以上要素，只要求我们去根据其中一个因素选择最合适路线，在我看来，这实际违背了我选择该题目
        //的初衷：因该在考虑到种种因素的印象下去选择最合适的“最短路径”
        //缺憾：遗憾的是，由于水平有限，我未能学会如何在有约束的条件下实现最短路径问题（在目前能力范围内和题目限制下，我没有能力去实现全局最优路径的选择）
        void calculate_Fit_Distance(std::string& origin,std::string& destination, Transport_Mode& mode, const std::string& user_start_time);
        void calculate_Fit_Cost(std::string& origin,std::string& destination, Transport_Mode& mode, const std::string& user_start_time);
        void calculate_Fit_Time(std::string& origin,std::string& destination, Transport_Mode& mode, const std::string& user_start_time);

        //析构函数
        ~Route_Graph() = default;
};

#endif