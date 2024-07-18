#include "Other.h"

//以下函数用于计算中转时间
//需要注意，在这里，我们只考虑了中转时间小于24小时的情况。

//解析字符串函数
std::tm parse_Time(const std::string &time)
{
    std::tm tmp_tm = {};
    std::istringstream ss(time);
    ss >> std::get_time(&tmp_tm, "%Y-%m-%d %H:%M");
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse time string");
    }
    return tmp_tm;
}

//计算中转时间
std::pair<int, int> calculate_Transfer_Time(const std::string &arrival_time_str, const std::string &start_time_str)
{
    try {
    // 将输入的时间字符串转换为tm格式
    std::tm arrival_time = parse_Time(arrival_time_str);
    std::tm start_time = parse_Time(start_time_str);
    // 将tm类型转换为time_point类型
    std::chrono::system_clock::time_point arrival_tp = std::chrono::system_clock::from_time_t(std::mktime(&arrival_time));
    std::chrono::system_clock::time_point start_tp = std::chrono::system_clock::from_time_t(std::mktime(&start_time));
    //计算时间差
    auto transfer_duration = start_tp - arrival_tp;
    //处理跨天的情况
    if (transfer_duration.count() < 0) {
        return {-1, -1};
    }
    //将时间间隔换为时分形式
    auto transfer_seconds = std::chrono::duration_cast<std::chrono::seconds>(transfer_duration).count();//将纳秒单位换成秒，存在数据截断
    int hours = transfer_seconds / 3600;
    int minutes = (transfer_seconds % 3600) / 60;
    return {hours, minutes};
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return {-1, -1};
    }
}

//计算花费时间
long long calculate_Time_In_Minutes(const std::string &time_str)
{
    std::tm time_tm = parse_Time(time_str);
    auto time_tp = std::chrono::system_clock::from_time_t(std::mktime(&time_tm));
    return std::chrono::duration_cast<std::chrono::minutes>(time_tp.time_since_epoch()).count();
}


//以下函数用于管理员登录
//"administrator_info.txt"是默认的储存账号信息的文件
bool read_Administrator_Infomation(std::string &administrator_name, std::string &password, std::string filename)
{
    std::ifstream account_file(filename);
    if(account_file.is_open()) {
        std::getline(account_file, administrator_name);
        std::getline(account_file, password);
        account_file.close();
        std::cout << "成功从文件读取身份信息" << std::endl;
        return true;
    }
    else {
        account_file.close();
        std::cerr << "无法打开身份信息文件" << std::endl;
        return false;
    }
}

bool authenticate(const std::string& input_Administrator_Name,  const std::string& input_Password)
{
    std::string administrator_name, password;
    if (read_Administrator_Infomation(administrator_name, password)) {
        return (input_Administrator_Name == administrator_name && input_Password == password);
    }
    std::cout << "您输入的用户信息不正确，请重新输入";
    return false;
}

// 主界面显示
void display_Main_Menu() {
    std::cout << "---------------------------------" << std::endl;
    std::cout << "欢迎使用交通路线管理系统" << std::endl;
    std::cout << "1. 以用户身份登录" << std::endl;
    std::cout << "2. 以管理员身份登录" << std::endl;
    std::cout << "3. 退出系统" << std::endl;
    std::cout << "请输入选项 (1-3): ";
}


//管理员界面显示
void display_Admin_Menu() {
    std::cout << "---------------------------------" << std::endl;
    std::cout << "管理员菜单" << std::endl;
    std::cout << "1. 增加路线" << std::endl;
    std::cout << "2. 删除路线" << std::endl;
    std::cout << "3. 修改路线" << std::endl;
    std::cout << "4. 查询路线" << std::endl;
    std::cout << "5. 增加城市" << std::endl;
    std::cout << "6. 删除城市" << std::endl;
    std::cout << "7. 查询城市" << std::endl;
    std::cout << "8. 返回主菜单" << std::endl;
    std::cout << "请输入选项 (1-8): ";
}

//用户界面显示
void display_User_Menu() {
    std::cout << "---------------------------------" << std::endl;
    std::cout << "用户菜单" << std::endl;
    std::cout << "1. 查询路线" << std::endl;
    std::cout << "2. 返回主菜单" << std::endl;
    std::cout << "请输入选项 (1-2): ";
}

//用户操作界面
void user_Interface(Route_Graph& route_graph) {
    User user;
    int choice = 0;
    //do-while 保证至少操作一次
    do {
        display_User_Menu();
        std::cin >> choice;
        switch (choice) {
            case 1:
                user.query(route_graph);
                break;
            case 2:
                user.quit();
                std::cout << "返回主菜单" << std::endl;
            default:
                std::cout << "无效选项，请重新输入" << std::endl;
        }
    } while (choice != 2);
}

//管理员操作界面
void admin_Interface(Route_Graph& route_graph) {
    std::string username, password;
    std::cout << "请输入管理员用户名: ";
    std::cin >> username;
    std::cout << "请输入管理员密码: ";
    std::cin >> password;

    if (authenticate(username, password)) {
        Administrator admin(username, password);
        int choice = 0;
        do {
            display_Admin_Menu();
            std::cin >> choice;
            switch (choice) {
                case 1:
                    admin.add_Route(route_graph);
                    break;
                case 2:
                    admin.delete_Route(route_graph);
                    break;
                case 3:
                    admin.modify_Route(route_graph);
                    break;
                case 4:
                    admin.consult_Route(route_graph);
                    break;
                case 5:
                    admin.add_City(route_graph);
                    break;
                case 6:
                    admin.delete_City(route_graph);
                    break;
                case 7:
                    admin.consult_City(route_graph);
                    break;
                case 8:
                    admin.quit();
                    std::cout << "返回主菜单" << std::endl;
                    break;
                default:
                    std::cout << "无效选项，请重新输入" << std::endl;
            }
        } while (choice != 8);
    } else {
        std::cout << "认证失败，返回主菜单" << std::endl;
    }
}