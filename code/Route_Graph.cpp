#include "Route_Graph.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<unordered_map>
#include<unordered_set>
#include<algorithm>
#include"Other.h"
// #include<cstdlib>
//City类定义

int City::next_city_id = 0;
int City::next_route_id = 0;
City::City(std::string name, double longitude, double altitude)
{
    this->city_id = ++next_city_id;
    this->name = name;
    this->longitude = longitude;
    this->altitude = altitude;
    route_table = std::list<std::list<Route>>();
}

void City::display_City_Information()
{
    std::cout << "city name: " << name << std::endl
              << "city id: " << city_id << std::endl
              << "city longitude and altitude : " << longitude << "  " << altitude << std::endl;
    display_City_Route_Table();
}          

void City::display_City_Route_Table()
{
    if(route_table.empty()) {
        std::cout << "该城市没有出行路线" << std::endl;
    }
    else {
        for (std::list<Route> routes : route_table) {
            if(!routes.empty())
            {
                for (Route& route : routes) {
                    route.display();
                }
            }
        }
    }
}


// Route类的定义
Route::Route(std::string start_city, std::string destination_city, std::string start_time, std::string arrival_time, float money_cost,std::string time_cost, float distance, Transport_Mode mode)
{
    this->start_city = start_city;
    this->destination_city = destination_city;
    this->start_time = start_time;
    this->arrival_time = arrival_time;
    this->money_cost = money_cost;
    this->time_cost = time_cost;
    this->distance = distance;
    this->mode = mode;
    id = 0;
}


Route::Route()
{
    this->start_city = "未命名";
    this->destination_city = "未命名";
    this->start_time = "未确定";
    this->arrival_time = "未确定";
    this->money_cost = 0.0;
    this->time_cost = "未确定";
    this->distance = 0.0;
    this->mode = Transport_Mode::none;
    id = 0;
}

Route::Route(const Route & route)
{
    this->start_city = route.start_city;
    this->destination_city = route.destination_city;
    this->start_time = route.start_time;
    this->arrival_time = route.arrival_time;
    this->money_cost = route.money_cost;
    this->time_cost = route.time_cost;
    this->distance = route.distance;
    this->mode = route.mode;
    id = route.id;
}

void Route::display()
{
    std::cout << "route_id: " << id << ", Start City: " << start_city << ", Destination City: " << destination_city
              << ", Start Time: " << start_time << ", Arrival Time: " << arrival_time
              << ", Time Cost: " << time_cost << ", Money Cost: " << money_cost
              << ", Distance: " << distance << ", Mode: " << transportModeToString(mode) << std::endl;
}

void Route::set_id(int id)
{
    this->id = id;
}

Route_Graph::Route_Graph()
{
    City_Count = 0;
    Route_Count = 0;
    Adjoin_Graph = std::vector<City>();
}

bool Route_Graph::initial_route_import(std::string& filename)
{
    std::ifstream data_file(filename);
    if (!data_file.is_open()) {
        std::cerr << "open failed:" << filename << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string data_line;
    while (std::getline(data_file, data_line)) {
        if(data_line.empty()) {
            continue;
        }
        std::istringstream ss(data_line);
        std::string type;//用于区分城市数据行和路线数据行
        ss >> type;

        if (type == "city" || type == "City") {
            //构造城市并加入,尚未考虑重复城市问题
            std::string name;
            if (std::find_if(Adjoin_Graph.begin(), Adjoin_Graph.end(), [&](const City& city){return city.name == name;}) == Adjoin_Graph.end()) {
                double longitude, altitude;
                ss >> name >> longitude >> altitude;
                City city(name, longitude, altitude);
                Adjoin_Graph.push_back(city);
                City_Count++;
            }
            else {
                std::cout << "城市已经添加，请更正数据后重启程序";
                data_file.close();
                // return false;
                exit(EXIT_FAILURE);
            }
        }
        else if (type == "route" || type == "Route") {
            //构造路线
            std::string tmp;//用于正确输入数据
            std::string start_city, destination_city, start_time, arrival_time, time_cost, mode_str;
            float money_cost, distance;
            ss >> start_city >> destination_city;
            //由于时间格式为2024-03-21 08:00，中间有空格，为了能够正确输入，必须做特殊处理
            ss >> start_time >> tmp;
            start_time += " " + tmp;
            ss >> arrival_time >> tmp;
            arrival_time += " " + tmp;
            ss >> money_cost >> time_cost >> distance >> mode_str;
            // Debug output for route info
            // std::cout << "Reading route: "
            //           << start_city << " " << destination_city << " " << start_time << " "
            //           << arrival_time << " " << money_cost << " " << time_cost << " " 
            //           << distance << " " << mode_str << std::endl;
            Transport_Mode mode = stringToTransportMode(mode_str);
            Route route(start_city, destination_city, start_time, arrival_time, money_cost, time_cost, distance, mode);
            route.set_id(++City::next_route_id);

            //查找起始城市是否存在，只有存在才将路线加入
            auto cityIter = std::find_if(Adjoin_Graph.begin(), Adjoin_Graph.end(), [&](const City& city) {
                return city.name == start_city;
            });
            if (cityIter != Adjoin_Graph.end()) {
                bool found = false;//标记是否找到到destination_city的路线
                for (auto& routeList : cityIter->route_table) {//注意只有引用类型才能修改循环变量值
                    if (!routeList.empty() && routeList.front().destination_city == destination_city) {//注意非空判断
                    routeList.push_back(route);
                    found = true;
                    break;
                    }
                }
                //如果存在城市但尚未有到目的城市的路径，则新创一个链表加入时刻表
                if (!found) {
                    std::list<Route> newRouteList = {route};
                    cityIter->route_table.push_back(newRouteList);
                }
                Route_Count++;
            }
            else {
                std::cout << "起点城市尚未添加，请添加后再尝试" << std::endl;
                data_file.close();
                exit(EXIT_FAILURE);
                // return false;
            }
        }
    }
    std::cout << "初始数据成功导入" << std::endl;
    data_file.close();
    return true;
}

bool Route_Graph::add_Route(Route& route)
{
    //寻找起始城市和终点城市的位置，并判断是否存在
    auto start_cityIter = std::find_if(Adjoin_Graph.begin(), Adjoin_Graph.end(), [&](const City& city) { return city.name == route.start_city;});
    auto destination_cityIter = std::find_if(Adjoin_Graph.begin(), Adjoin_Graph.end(), [&](const City& city) { return city.name == route.destination_city;});
    //当起点终点均存在时，执行操作
    if (start_cityIter != Adjoin_Graph.end() && destination_cityIter != Adjoin_Graph.end()) {
        bool found = false;//用来标记是否找到一条已存在的由起点到终点的路线
        route.set_id(++City::next_route_id);
        for (auto& routeList : start_cityIter->route_table) {
            if (!routeList.empty() && routeList.front().destination_city == route.destination_city) {
                routeList.push_back(route);
                found = true;
                break;
            }
        }
        if (!found) {
            std::list<Route> new_Route_List = {route};
            start_cityIter->route_table.push_back(new_Route_List);
        }
        Route_Count++;
    }
    else {
        std::cout << "Error: Start city or destination city not found.add failed" << std::endl;
        return false;
    }
    return true;
}

bool Route_Graph::delete_Route(std::string& origin, std::string& destination, int& route_id)
{
    auto cityIter = std::find_if(Adjoin_Graph.begin(), Adjoin_Graph.end(), [&](const City& city){return origin == city.name;});
    if (cityIter != Adjoin_Graph.end()) {
        for (auto& routeList : cityIter->route_table) {
            if (!routeList.empty() && routeList.front().destination_city == destination) {
                auto routeIter = std::find_if(routeList.begin(), routeList.end(), [&](const Route& route) {return route.id == route_id;});
                if (routeIter != routeList.end()) {
                    routeList.erase(routeIter);
                    Route_Count--;
                }
            }
        }
    }
    else {
        std::cerr << "Error: Route not found.delete failed" << std::endl;
        return false;
    }
    return true;
}

bool Route_Graph::modify_Route(std::string& origin, std::string& destination, int& route_id)
{
    auto cityIter = std::find_if(Adjoin_Graph.begin(), Adjoin_Graph.end(), [&](const City& city){return city.name == origin;});
    if (cityIter != Adjoin_Graph.end()) {
        for (auto& routelist : cityIter->route_table) {
            if (!routelist.empty() && routelist.front().destination_city == destination) {
                auto routeIter = std::find_if(routelist.begin(), routelist.end(), [&](const Route& route){return route.id == route_id;} );
                if (routeIter != routelist.end()) {
                    std::cout << "请选择你要执行的修改操作(输入1或2): 1: 修改时间 2: 修改费用" << std::endl;
                    int selection;
                    std::cin >> selection;
                    switch (selection) {
                        case 1:
                        {
                            std::string start_time, arrival_time, time_cost;
                            std::string tmp;//用于正确输入数据
                            //由于时间格式为2024-03-21 08:00，中间有空格，为了能够正确输入，必须做特殊处理
                            std::cout << "请输入新的起始时间(输入样式: 2024-03-07 08:05): ";
                            std::cin >> std::ws >> start_time >> tmp;
                            start_time += " " + tmp;
                            std::cout << "请输入新的抵达时间(输入样式: 2024-03-07 12:05): ";
                            std::cin >> std::ws >> arrival_time >> tmp;
                            arrival_time += " " + tmp;
                            std::cout << "请输入新的花费时间(输入样式: 2 hours 5 minutes): ";
                            std::cin >> std::ws;
                            std::getline(std::cin, time_cost);
                            routeIter->start_time = start_time;
                            routeIter->arrival_time = arrival_time;
                            routeIter->time_cost = time_cost;
                            break;
                        }
                        case 2:
                        {
                            std::cout << "请输入新的费用(元): ";
                            float cost;
                            std::cin >> cost;
                            routeIter->money_cost = cost;
                            break;
                        }
                        default:
                        {
                            std::cout << "错误输入" << std::endl;
                            return false;
                        }
                    }
                    return true;
                }
            } 
        }
    }
    std::cerr << "Error: Route not found.modify failed" << std::endl;
    return false;
}

bool Route_Graph::consult_Route(std::string& origin, std::string& destination, int& route_id)
{
    auto cityIter = std::find_if(Adjoin_Graph.begin(), Adjoin_Graph.end(), [&](const City& city){return city.name == origin;});
    if (cityIter != Adjoin_Graph.end()) {
        for (auto& routelist : cityIter->route_table) {
            if (!routelist.empty() && routelist.front().destination_city == destination) {
                auto routeIter = std::find_if(routelist.begin(), routelist.end(), [&](const Route& route){return route.id == route_id;} );
                if (routeIter != routelist.end()) {
                    routeIter->display();
                    return true;
                }
            } 
        }
    }
    std::cerr << "Error: Route not found.consult failed" << std::endl;
    return false;
}

bool Route_Graph::add_City(std::string& city_name, double& longitude, double& altitude)
{
    //如果城市已存在，添加失败
    auto cityIter = std::find_if(Adjoin_Graph.begin(), Adjoin_Graph.end(), [&](const City& city){return city.name == city_name;});
        if (cityIter != Adjoin_Graph.end()) {
        std::cerr << "Error: City already exists." << std::endl;
        return false;
    }
    City new_city(city_name, longitude, altitude);
    Adjoin_Graph.push_back(new_city);
    City_Count++;
    return true;
}

bool Route_Graph::delete_City(std::string& city_name)
{
    auto cityIter = std::find_if(Adjoin_Graph.begin(), Adjoin_Graph.end(), [&](const City& city){return city.name == city_name;});
    if (cityIter != Adjoin_Graph.end()) {
        //删除城市
        Adjoin_Graph.erase(cityIter);
        City_Count--;

        //更新路线
        //删除所有指向该城市的路线
        for (auto& city : Adjoin_Graph) {
            for (auto route_list_iter = city.route_table.begin(); route_list_iter != city.route_table.end();) {
                for (auto route_iter = route_list_iter->begin(); route_iter != route_list_iter->end();) {
                    if (route_iter->destination_city == city_name) {
                        route_iter = route_list_iter->erase(route_iter);//erase返沪指向删除元素的下一个元素的迭代器
                        Route_Count--;
                    }
                    else {
                        ++route_iter;
                    }
                }
                //如果删除后路线列表为空，则删除这个链表
                if (route_list_iter->empty()) {
                    route_list_iter = city.route_table.erase(route_list_iter);
                }
                else {
                    ++route_list_iter;
                }
            }
        }
        return true;
    }
    
    std::cerr << "Error: City not found.delete city failed" << std::endl;
    return false;
}

void Route_Graph::consult_City(std::string& city_name)
{

    // 在 Adjoin_Graph 中找到城市名称为 city_name 的城市
    auto cityIter = std::find_if(Adjoin_Graph.begin(), Adjoin_Graph.end(), [&city_name](const City& city) {
        return city.name == city_name;
    });

    // 如果找到了城市，则输出城市信息和路线表
    if (cityIter != Adjoin_Graph.end()) {
        // 显示城市信息
        cityIter->display_City_Information();
    } 
    else {
        // 如果未找到城市，则输出错误信息
        std::cerr << "Error: City not found.consult city failed" << std::endl;
    }
}

void Route_Graph::calculate_Fit_Distance(std::string& origin, std::string& destination,Transport_Mode& mode, const std::string& user_start_time)
{
    std::unordered_map<std::string, float> distances; //储存到各城市的最短距离
    std::unordered_map<std::string, std::string> previous_city;//储存最短路径上的前一个城市
    std::unordered_map<std::string, std::string> previous_arrival_time;
    std::unordered_map<std::string, std::pair<std::string, std::string>> route_times;//记录当前路线的起止时间
    auto cmp = [&](std::string left, std::string right) {return distances[left] > distances[right];};//用于比较两个城市距离的Lambda函数
    std::priority_queue<std::string, std::vector<std::string>, decltype(cmp)> pq(cmp);//优先队列进行最小距离选取
    std::unordered_set<std::string> in_Queue;//标志是否在队列中
    std::unordered_set<std::string> processed;//标志是否已经选取过
    
    //判断起点终点是否存在已添加城市中
    auto siter = std::find_if(Adjoin_Graph.begin(), Adjoin_Graph.end(), [&](const City& city) {return city.name == origin;});
    auto diter = std::find_if(Adjoin_Graph.begin(), Adjoin_Graph.end(), [&](const City& city) {return city.name == destination;});
    if (siter == Adjoin_Graph.end()) {
        std::cout << "起点城市不存在" << std::endl;
        return;
    }
    else if (diter == Adjoin_Graph.end()) {
        std::cout << "终点城市不存在" << std::endl;
        return;
    }

    //设置初始距离
    for (const auto& city : Adjoin_Graph) {
        distances[city.name] = std::numeric_limits<float>::infinity();//将初始距离设置为浮点数正无穷
    }

    //将起点城市距离设置为0，并加入优先队列和已访问队列
    distances[origin] = 0;
    pq.push(origin);
    in_Queue.insert(origin);
    previous_arrival_time[origin] = user_start_time;//设置为用户出发时间，可以实现起始城市路线选择

    //处理优先队列中的城市
    while (!pq.empty()){

        //当队列不为空时，获取当前最短距离城市并将其踢出队列
        std::string current_city = pq.top();
        pq.pop();

        //更新状态：设置当前城市已处理和不在队列中
        in_Queue.erase(current_city);
        processed.insert(current_city);

        //如果找到目标城市，直接退出，剪枝操作
        if (current_city == destination) {
            break;
        }

        //找到当前城市位置
        auto City_Iter = std::find_if(Adjoin_Graph.begin(), Adjoin_Graph.end(), [&](const City& city) {return city.name == current_city;});

        //如果找到当前城市，遍历当前城市所有路线
        if (City_Iter != Adjoin_Graph.end()) {
            for (const auto& route_list : City_Iter->route_table) {
                Route best_route = route_list.front();//找到的最短距离路线
                bool found_valid_route = false;//标志是否找到方式符合的最短路径
                for (const auto& route : route_list) {
                    //交通方式不符合，跳过
                    if (route.mode != mode) {
                        continue;
                    }
                    //计算当前路线的启程时间和到该城市的抵达时间之间的时间差，若为负值，则不可选取，跳过。
                    auto transfer_time = calculate_Transfer_Time(previous_arrival_time[current_city], route.start_time);
                    if (transfer_time.first < 0 || transfer_time.second < 0) {
                            continue;
                    }
                    //如果当前路线为目的地为已经处理过的城市，则跳过
                    if (processed.find(route.destination_city) != processed.end()) {
                        continue;
                    }
                    //若为距离更短的路径，将其设置为最佳路径
                    if (route.distance < best_route.distance || !found_valid_route) {
                        best_route = route;
                        found_valid_route = true;
                    }
                }
                //如果交通方式符合，执行操作
                if (found_valid_route) {
                    //计算通过当前城市到达邻居城市的新距离
                    float new_distance = distances[current_city] + best_route.distance;
                    //如果新距离小于当前记录的邻居城市的距离，则更新邻居城市的距离，并更改其最短路径上的前一个城市为当前城市，同时更新目标城市的抵达时间
                    if (new_distance < distances[best_route.destination_city]) {
                        distances[best_route.destination_city] = new_distance;
                        previous_city[best_route.destination_city] = current_city;
                        previous_arrival_time[best_route.destination_city] = best_route.arrival_time;
                        route_times[best_route.destination_city] = {best_route.start_time, best_route.arrival_time};

                        //如果当前城市已经加入过优先队列，则不再加入，否则加入
                        //通过这种方法，可以避免重复添加城市，确保每个城市在优先队列中最多出现一次，从而提高算法的效率
                        if (in_Queue.find(best_route.destination_city) == in_Queue.end()) {
                            pq.push(best_route.destination_city);
                            in_Queue.insert(best_route.destination_city);
                        }
                    }
                }
            }
        }
    }

    //如果目标城市的最短距离仍然是无穷大，则表示没有路径从起点城市到达目标城市，输出相应信息
    if (distances[destination] == std::numeric_limits<float>::infinity()) {
        std::cout << "no route from " << origin << " to " << destination << "." << std::endl;
    }
    
    //否则，输出从起点城市到目标城市的最短距离。
    else {
        std::cout << "the shortest distance from " << origin << " to " << destination << " is " << distances[destination] << "." << std::endl;
        std::vector<std::string> path;
        //通过 previous 哈希表回溯路径，将路径存储在 path 向量中。
        for (std::string city = destination; city != origin; city = previous_city[city]) {
            path.push_back(city);
        }
        //加入起点城市
        path.push_back(origin);
        //反转 path 向量以得到从起点城市到目标城市的正确顺序，并输出路径。
        std::reverse(path.begin(), path.end());
        std::cout << "Path: ";
        for (const auto& city : path) {
            std::cout << city << " ";
        }
        std::cout << std::endl;
        // 输出路径上的各段出发时间和抵达时间
        std::cout << "Route details:" << std::endl;
        std::cout << "user start time: " << user_start_time << std::endl;
        for (size_t i = 1; i < path.size(); ++i) {
            std::string from_city = path[i-1];
            std::string to_city = path[i];
            std::cout << "From " << from_city << " to " << to_city
                      << " | Start: " << route_times[to_city].first
                      << " | Arrival: " << route_times[to_city].second << std::endl;
        }
    }
}


//具体解释见最短距离函数
void Route_Graph::calculate_Fit_Cost(std::string& origin, std::string& destination, Transport_Mode& mode, const std::string& user_start_time)
{
    std::unordered_map<std::string, float> costs;//记录到各个城市的最少费用
    std::unordered_map<std::string, std::string> previous_city;//记录最短路径上的前一个城市
    std::unordered_map<std::string, std::string> previous_arrival_time;
    std::unordered_map<std::string, std::pair<std::string, std::string>> route_times;//记录当前路线的起止时间
    auto cmp = [&](std::string left, std::string right) { return costs[left] > costs[right];};
    std::priority_queue<std::string, std::vector<std::string>, decltype(cmp)> pq(cmp);
    std::unordered_set<std::string> in_Queue;
    std::unordered_set<std::string> processed;

    auto siter = std::find_if(Adjoin_Graph.begin(), Adjoin_Graph.end(), [&](const City& city) {return city.name == origin;});
    auto diter = std::find_if(Adjoin_Graph.begin(), Adjoin_Graph.end(), [&](const City& city) {return city.name == destination;});
    if (siter == Adjoin_Graph.end()) {
        std::cout << "起点城市不存在" << std::endl;
        return;
    }
    else if (diter == Adjoin_Graph.end()) {
        std::cout << "终点城市不存在" << std::endl;
        return;
    }

    for (const auto& city : Adjoin_Graph) {
        costs[city.name] = std::numeric_limits<float>::infinity();
    }
    
    costs[origin] = 0;
    pq.push(origin);
    in_Queue.insert(origin);

    // auto it = std::find_if(Adjoin_Graph.begin(), Adjoin_Graph.end(), [&](const City& city) {return city.name == origin;});
    previous_arrival_time[origin] = user_start_time;
    
    while (!pq.empty()) {
        std::string current_city = pq.top();
        pq.pop();
        in_Queue.erase(current_city);
        processed.insert(current_city);

        if (current_city == destination) {
            break;
        }

        auto it = std::find_if(Adjoin_Graph.begin(), Adjoin_Graph.end(), [&](const City& city) {return city.name == current_city;});

        if (it != Adjoin_Graph.end()) {
            for (const auto& route_list : it->route_table) {
                // 找到费用最低的路线
                Route best_route = route_list.front();
                bool found_valid_route = false;
                for (const auto& route : route_list) {
                    if (route.mode != mode) {
                        continue;
                    }
                    auto transfer_time = calculate_Transfer_Time(previous_arrival_time[current_city], route.start_time);
                    if (transfer_time.first < 0 || transfer_time.second < 0) {
                        continue;
                    }
                    if (processed.find(route.destination_city) != processed.end()) {
                        continue;
                    }
                    if (route.money_cost < best_route.money_cost || !found_valid_route) {
                        best_route = route;
                        found_valid_route = true;
                    }
                }                    
                // 如果找到了有效的路线，则更新费用和路径
                if (found_valid_route) {
                    auto transfer_time = calculate_Transfer_Time(previous_arrival_time[current_city], best_route.start_time);
                    if (transfer_time.first < 0 || transfer_time.second < 0) {
                        continue;
                    }

                    float new_cost = costs[current_city] + best_route.money_cost;
                    if (new_cost < costs[best_route.destination_city]) {
                        costs[best_route.destination_city] = new_cost;
                        previous_city[best_route.destination_city] = current_city;
                        previous_arrival_time[best_route.destination_city] = best_route.arrival_time;
                        route_times[best_route.destination_city] = {best_route.start_time, best_route.arrival_time};

                        if (in_Queue.find(best_route.destination_city) == in_Queue.end()) {
                            pq.push(best_route.destination_city);
                            in_Queue.insert(best_route.destination_city);
                        }
                    }
                }
            }
        }
    }


    if (costs[destination] == std::numeric_limits<double>::infinity()) {
        std::cout << "No route from " << origin << " to " << destination << "." << std::endl;
    } 
    else {
        std::cout << "The lowest cost from " << origin << " to " << destination << " is " << costs[destination] << "." << std::endl;
        std::vector<std::string> path;
        for (std::string at = destination; at != origin; at = previous_city[at]) {
            path.push_back(at);
        }
        path.push_back(origin);
        std::reverse(path.begin(), path.end());
        std::cout << "Path: ";
        for (const auto& city : path) {
            std::cout << city << " ";
        }
        std::cout << std::endl;
        // 输出路径上的各段出发时间和抵达时间
        std::cout << "Route details:" << std::endl;
        std::cout << "user start time: " << user_start_time << std::endl;
        for (size_t i = 1; i < path.size(); ++i) {
            std::string from_city = path[i-1];
            std::string to_city = path[i];
            std::cout << "From " << from_city << " to " << to_city
                      << " | Start: " << route_times[to_city].first
                      << " | Arrival: " << route_times[to_city].second << std::endl;
        }
    }
}


//详细注释见最短距离函数
//在当前实现上，我们计算的是从用户指定时间开始到最终抵达时间的总时间最短，而不是从路途出发开始到抵达时间的最短
void Route_Graph::calculate_Fit_Time(std::string& origin, std::string& destination, Transport_Mode& mode, const std::string& user_start_time)
{
    std::unordered_map<std::string, float> times;//记录到各个城市的最少时间
    std::unordered_map<std::string, std::string> previous_city;//记录最短路径上的前一个城市
    std::unordered_map<std::string, std::string> previous_arrival_time;//记录当前城市最短路径的抵达时间
    std::unordered_map<std::string, std::pair<std::string, std::string>> route_times;//记录当前路线的起止时间
    auto cmp = [&](std::string left, std::string right) { return times[left] > times[right];};//比较函数
    std::priority_queue<std::string, std::vector<std::string>, decltype(cmp)> pq(cmp);
    std::unordered_set<std::string> in_Queue;//标记是否加入队列
    std::unordered_set<std::string> processed;//标记是否已经访问

    auto siter = std::find_if(Adjoin_Graph.begin(), Adjoin_Graph.end(), [&](const City& city) {return city.name == origin;});
    auto diter = std::find_if(Adjoin_Graph.begin(), Adjoin_Graph.end(), [&](const City& city) {return city.name == destination;});
    if (siter == Adjoin_Graph.end()) {
        std::cout << "起点城市不存在" << std::endl;
        return;
    }
    else if (diter == Adjoin_Graph.end()) {
        std::cout << "终点城市不存在" << std::endl;
        return;
    }

    for (const auto& city : Adjoin_Graph) {
        times[city.name] = std::numeric_limits<float>::infinity();
    }
    times[origin] = 0;
    pq.push(origin);
    in_Queue.insert(origin);
    previous_arrival_time[origin] = user_start_time;

    while (!pq.empty()) {
        std::string current_city = pq.top();
        pq.pop();
        in_Queue.erase(current_city);
        processed.insert(current_city);

        if (current_city == destination) {
            break;
        }
        auto City_Iter = std::find_if(Adjoin_Graph.begin(), Adjoin_Graph.end(), [&](const City& city) {return city.name == current_city;});
        if (City_Iter != Adjoin_Graph.end()) {
            for (const auto& route_list : City_Iter->route_table) {
                // 找到费用最低的路线
                Route best_route = route_list.front();
                bool found_valid_route = false;
                for (const auto& route : route_list) {
                    if (route.mode != mode) {
                        continue;
                    }
                    auto transfer_time = calculate_Transfer_Time(previous_arrival_time[current_city], route.start_time);
                    if (transfer_time.first < 0 || transfer_time.second < 0) {
                        continue;
                    }
                    if (processed.find(route.destination_city) != processed.end()) {
                        continue;
                    }
                    //计算选择当前路线的花费时间 = 路途时间+中转时间
                    long long route_previous_arrival_minutes = calculate_Time_In_Minutes(previous_arrival_time[current_city]);//当前路线到目标城市的抵达时间
                    long long route_arrival_minutes = calculate_Time_In_Minutes(route.arrival_time);//已得到最短路线到达当前城市的抵达时间时间
                    float total_time_cost = route_arrival_minutes - route_previous_arrival_minutes;//二者相减得到增加的花费时间
                    
                    //如果当前花费时间更少或找到第一条交通方式符合的路线，则更新最佳路线
                    if (!found_valid_route || total_time_cost < (calculate_Time_In_Minutes(best_route.arrival_time) - calculate_Time_In_Minutes(previous_arrival_time[current_city]))) {
                        best_route = route;
                        found_valid_route = true;
                    }
                }

                //如果找到可行路线
                if (found_valid_route) {
                    long long route_previous_arrival_minutes = calculate_Time_In_Minutes(previous_arrival_time[current_city]);
                    long long route_arrival_minutes = calculate_Time_In_Minutes(best_route.arrival_time);
                    float new_time = times[current_city] + (route_arrival_minutes - route_previous_arrival_minutes);
                    if (new_time < times[best_route.destination_city]) {
                        times[best_route.destination_city] = new_time;
                        previous_city[best_route.destination_city] = current_city;
                        previous_arrival_time[best_route.destination_city] = best_route.arrival_time;
                        route_times[best_route.destination_city] = {best_route.start_time, best_route.arrival_time};

                        if (in_Queue.find(best_route.destination_city) == in_Queue.end()) {
                            pq.push(best_route.destination_city);
                            in_Queue.insert(best_route.destination_city);
                        }
                    }
                }
            }                    
        }
    }
    if (times[destination] == std::numeric_limits<float>::infinity()) {
        std::cout << "No route from " << origin << " to " << destination << "." << std::endl;
    } 
    else {
        std::cout << "The shortest travel time from " << origin << " to " << destination << " is " << int(times[destination]) / 60 << " hours " << int(times[destination]) % 60 << " minutes." << std::endl;
        std::vector<std::string> path;
        for (std::string city = destination; city != origin; city = previous_city[city]) {
            path.push_back(city);
        }
        path.push_back(origin);
        std::reverse(path.begin(), path.end());
        std::cout << "Path: ";
        for (const auto& city : path) {
            std::cout << city << " ";
        }
        std::cout << std::endl;
        // 输出路径上的各段出发时间和抵达时间
        std::cout << "Route details:" << std::endl;
        std::cout << "user start time: " << user_start_time << std::endl;
        for (size_t i = 1; i < path.size(); ++i) {
            std::string from_city = path[i-1];
            std::string to_city = path[i];
            std::cout << "From " << from_city << " to " << to_city
                      << " | Start: " << route_times[to_city].first
                      << " | Arrival: " << route_times[to_city].second << std::endl;
        }
    }
}
