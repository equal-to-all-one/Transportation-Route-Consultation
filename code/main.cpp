#include <iostream>
#include <fstream>
#include <string>
#include "Route_Graph.h"
#include "User.h"
#include "Administrator.h"
#include "Transway.h"
#include "Other.h"
using namespace std;

int main() {
    Route_Graph route_graph;
    string data_file = "data.txt";
    route_graph.initial_route_import(data_file);
    int choice = 0;
    do {
        display_Main_Menu();
        std::cin >> choice;
        switch (choice) {
            case 1:
                user_Interface(route_graph);
                break;
            case 2:
                admin_Interface(route_graph);
                break;
            case 3:
                std::cout << "退出系统" << std::endl;
                break;
            default:
                std::cout << "无效选项，请重新输入" << std::endl;
        }
    } while (choice != 3);

    return 0;
}

// #include "Route_Graph.h"
// #include "Administrator.h"
// #include "User.h"
// #include "Other.h"
// #include <iostream>
// #include <string>

// // Helper function to print a line separator
// void print_separator() {
//     std::cout << "==============================" << std::endl;
// }

// // Test adding a city
// void test_add_city(Route_Graph& route_graph) {
//     print_separator();
//     std::cout << "=== Test Adding City ===" << std::endl;
//     std::string city_name = "Nanjing";
//     double longitude = 118.78;
//     double altitude = 32.04;
//     route_graph.add_City(city_name, longitude, altitude);
//     route_graph.consult_City(city_name);
// }

// // Test adding a route
// void test_add_route(Route_Graph& route_graph) {
//     print_separator();
//     std::cout << "=== Test Adding Route ===" << std::endl;
//     Route new_route("Beijing", "Nanjing", "2024-03-22 10:00", "2024-03-22 12:00", 200, "2 hours", 300, Transport_Mode::airplane);
//     route_graph.add_Route(new_route);
//     int route_id = new_route.id;
//     std::string n1 = "Beijing",n2 =  "Nanjing";
//     route_graph.consult_Route(n1,n2, route_id);
// }

// // Test deleting a city
// void test_delete_city(Route_Graph& route_graph) {
//     print_separator();
//     std::cout << "=== Test Deleting City ===" << std::endl;
//     std::string city_name = "Nanjing";
//     route_graph.delete_City(city_name);
//     route_graph.consult_City(city_name);
// }

// // Test modifying a route
// void test_modify_route(Route_Graph& route_graph) {
//     print_separator();
//     std::cout << "=== Test Modifying Route ===" << std::endl;
//     std::string origin = "Beijing";
//     std::string destination = "Shanghai";
//     int route_id = 1;
//     route_graph.modify_Route(origin, destination, route_id);
//     route_graph.consult_Route(origin, destination, route_id);
// }

// // Test querying a route
// void test_query_route(Route_Graph& route_graph) {
//     print_separator();
//     std::cout << "=== Test Querying Route ===" << std::endl;
//     std::string origin = "Beijing";
//     std::string destination = "Shanghai";
//     int route_id = 1;
//     route_graph.consult_Route(origin, destination, route_id);
// }

// // Test user query functionality
// void test_user_query(Route_Graph& route_graph) {
//     print_separator();
//     std::cout << "=== Test User Query ===" << std::endl;
//     User user;
//     user.query(route_graph);
// }

// // Test shortest path calculation
// void test_shortest_path(Route_Graph& route_graph) {
//     std::string start_city = "Beijing";
//     std::string destination_city = "Shenzhen";
//     Transport_Mode mode = Transport_Mode::airplane;
//     std::string start_time = "2024-03-21 02:00";

//     print_separator();
//     std::cout << "=== Test Shortest Path Calculation (Distance) ===" << std::endl;
//     route_graph.calculate_Fit_Distance(start_city, destination_city, mode, start_time);

//     print_separator();
//     std::cout << "=== Test Shortest Path Calculation (Cost) ===" << std::endl;
//     route_graph.calculate_Fit_Cost(start_city, destination_city, mode, start_time);

//     print_separator();
//     std::cout << "=== Test Shortest Path Calculation (Time) ===" << std::endl;
//     route_graph.calculate_Fit_Time(start_city, destination_city, mode, start_time);
// }

// int main() {
//     Route_Graph route_graph;
//     std::string data_file = "data.txt";
//     route_graph.initial_route_import(data_file);

//     // // Test adding a city
//     // test_add_city(route_graph);

//     // // Test adding a route
//     // test_add_route(route_graph);

//     // // Test deleting a city
//     // test_delete_city(route_graph);

//     // // Test modifying a route
//     // test_modify_route(route_graph);

//     // // Test querying a route
//     // test_query_route(route_graph);

//     // // Test user query functionality
//     // test_user_query(route_graph);

//     // Test shortest path calculation
//     test_shortest_path(route_graph);

//     return 0;
// }