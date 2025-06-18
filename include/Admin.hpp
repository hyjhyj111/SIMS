#pragma once
#include <string>
#include <Database.hpp>
#include <utility>
#include <termios.h>
#include <unistd.h>
class Admin {
public:
    Admin() = default;

    Admin(std::string username) : username(std::move(username)) {}

    std::string get_username() const { return username; }
    bool check_password(const std::string& password) const {
        Database db;
        return db.admin_exit(username, password);
    }

private:
    std::string username;
};
inline std::string read_password() {
    termios oldt, newt;

    // 获取终端设置
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // 关闭回显
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    std::string password;

    // 加这个忽略前面输入残留的换行符
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::getline(std::cin, password);

    // 恢复终端设置
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << std::endl;

    return password;
}