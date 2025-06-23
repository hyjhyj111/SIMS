#include <bits/stdc++.h>
#include <Manager.hpp>
#include <Admin.hpp>
#include <Logger.hpp>
#include <fmt/format.h>
void solve() {
    bool login = false, EXIT = false;
    std::string username;
    while (!EXIT) {
        if (!login) {
            std::string password;
            std::cout << "请输入账号:";
            std::cin >> username;
            std::cout << "请输入密码:";
            password = read_password();
            if (Admin(username).check_password(password)) {
                login = true;
                std::cout << "登陆成功" << std::endl;
                LOG_INFO(fmt::format("{} login successfully", username));
            } else {
                std::cout << "账号密码错误" << std::endl;
                LOG_INFO(fmt::format("{} login failure", username));
            }
            continue;
        }

        manager::menu();
        int op;
        std::cin >> op;
        switch (op) {
            case 0:
                EXIT = true;
                break;
            case 1:
                manager::add_student();
                break;
            case 2:
                manager::show_student();
                break;
            case 3:
                manager::delete_student();
                break;
            case 4:
                manager::edit_student();
                break;
            case 5:
                manager::find_student();
                break;
            case 6:
                manager::show_sorted();
                break;
            case 7:
                manager::add_admin();
                break;
            case 8:
                manager::log_out(login, username);
            default:
                system("clear");
                break;
        }
    }
}
int main() {
    Logger::Init();
    solve();
    Logger::Shutdown();
    return 0;
}
