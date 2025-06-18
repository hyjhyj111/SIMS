#include <fmt/core.h>
#include <Manager.hpp>
#include <Database.hpp>
#include <iostream>
#include <Logger.hpp>
#include <Admin.hpp>
void manager::menu() {
    std::cout << "********************************************" << std::endl;
    std::cout << "**********  欢迎使用学生管理系统！ *********" << std::endl;
    std::cout << "*************  0.退出管理程序  *************" << std::endl;
    std::cout << "*************  1.增加学生信息  *************" << std::endl;
    std::cout << "*************  2.显示学生信息  *************" << std::endl;
    std::cout << "*************  3.删除学生信息  *************" << std::endl;
    std::cout << "*************  4.修改学生信息  *************" << std::endl;
    std::cout << "*************  5.查找学生信息  *************" << std::endl;
    std::cout << "*************  6.按照学号排序  *************" << std::endl;
    std::cout << "*************  7.增加管理账户  *************" << std::endl;
    std::cout << "********************************************" << std::endl;
    std::cout << std::endl;
}
void manager::add_student() {
    fmt::print("请输入要添加的学生的学号: ");
    int id;
    std::cin >> id;

    Database database;
    if (database.id_exit(id)) {
        fmt::print("ID 已存在，无法添加。\n");
        LOG_INFO(fmt::format("添加失败：学生 ID {} 已存在", id));
        return;
    }

    std::string name, major;
    int age, sex;

    fmt::print("请输入学生姓名: ");
    std::cin >> name;

    fmt::print("请输入学生专业: ");
    std::cin >> major;

    fmt::print("请输入学生年龄: ");
    std::cin >> age;

    fmt::print("请输入学生性别 (0: 女, 1: 男): ");
    std::cin >> sex;

    Student s{name, major, id, age, sex};
    database.insert_student(s);

    fmt::print("学生添加成功！\n");
    LOG_INFO(fmt::format("成功添加学生：{}（ID: {}）", name, id));
}

void manager::show_student() {
    Database database;
    for (const auto &student : database.students()) {
        fmt::println("{}", student);
    }
}


void manager::delete_student() {
    Database database;
    int id;
    fmt::print("请输入要删除的学生 ID：");
    std::cin >> id;

    if (!database.id_exit(id)) {
        fmt::print("该学生不存在。\n");
        LOG_INFO(fmt::format("尝试删除不存在的学生 ID: {}", id));
        return;
    }

    Student s = database.select_by_id(id);
    fmt::print("确认删除学生：{} ？(y/n)：", s);

    char confirm;
    std::cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        if (database.delete_by_id(id)) {
            fmt::print("删除成功。\n");
            LOG_INFO(fmt::format("成功删除学生：{} ", s));
        } else {
            fmt::print("删除失败。\n");
            LOG_ERROR(fmt::format("删除学生失败：{} ", s));
        }
    } else {
        fmt::print("已取消删除。\n");
        LOG_INFO(fmt::format("取消删除学生：{}", s));
    }
}

void manager::edit_student() {
    fmt::print("请输入要修改的学生 ID: ");
    int id;
    std::cin >> id;

    Database database;
    if (!database.id_exit(id)) {
        fmt::print("未找到对应的学生 ID {}\n", id);
        LOG_WARNING(fmt::format("修改失败：学生 ID {} 不存在", id));
        return;
    }

    Student s = database.select_by_id(id);
    fmt::print("当前学生信息：\n");
    fmt::println("{}", s);

    std::string input;

    fmt::print("是否修改姓名？(y/n): ");
    std::cin >> input;
    if (input == "y" || input == "Y") {
        fmt::print("请输入新姓名: ");
        std::cin >> s.name;
    }

    fmt::print("是否修改专业？(y/n): ");
    std::cin >> input;
    if (input == "y" || input == "Y") {
        fmt::print("请输入新专业: ");
        std::cin >> s.major;
    }

    fmt::print("是否修改年龄？(y/n): ");
    std::cin >> input;
    if (input == "y" || input == "Y") {
        fmt::print("请输入新年龄: ");
        std::cin >> s.age;
    }

    fmt::print("是否修改性别？(y/n): ");
    std::cin >> input;
    if (input == "y" || input == "Y") {
        fmt::print("请输入新性别 (0: 女, 1: 男): ");
        std::cin >> s.sex;
    }

    // 删除旧记录再插入新记录
    if (!database.delete_by_id(id)) {
        fmt::print("修改失败：删除原始数据失败\n");
        LOG_ERROR(fmt::format("修改失败：无法删除 ID {}", id));
        return;
    }

    database.insert_student(s);
    fmt::print("学生信息已修改成功！\n");
    LOG_INFO(fmt::format("学生 ID {} 信息修改成功", id));
}

void manager::find_student() {
    fmt::print("请输入要查找的学生 ID: ");
    int id;
    std::cin >> id;

    Database database;
    if (!database.id_exit(id)) {
        fmt::print("未找到学生 ID 为 {} 的学生。\n", id);
        LOG_WARNING(fmt::format("查找失败：学生 ID {} 不存在", id));
        return;
    }

    Student s = database.select_by_id(id);
    fmt::print("查找到的学生信息如下：\n");
    fmt::println("{}", s);
    LOG_INFO(fmt::format("成功查找学生：{}", s));
}

void manager::show_sorted() {
    Database database;
    auto students = database.students();

    fmt::println("请选择排序方式:");
    fmt::println("1. 按学号排序");
    fmt::println("2. 按姓名排序");
    fmt::println("3. 按年龄排序");
    fmt::print("请输入选项（数字）：");

    int option = 0;
    std::cin >> option;

    switch (option) {
        case 1:
            std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
                return a.id < b.id;
            });
            break;
        case 2:
            std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
                return a.name < b.name;
            });
            break;
        case 3:
            std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
                return a.age < b.age;
            });
            break;
        default:
            fmt::println("无效选项，默认按学号排序。");
            std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
                return a.age < b.age;
            });
            break;
    }

    fmt::println("排序后的学生列表：");
    for (const auto& student : students) {
        fmt::println("{}", student);
    }
    LOG_INFO(fmt::format("按选项 {} 排序并显示学生列表。", option));
}

void manager::add_admin() {
    fmt::print("请输入新管理员用户名: ");
    std::string username;
    std::cin >> username;

    fmt::print("请输入新管理员密码: ");
    std::string password = read_password();

    if (username.empty() || password.empty()) {
        fmt::print("用户名或密码不能为空！\n");
        LOG_WARNING("添加管理员失败，用户名或密码为空");
        return;
    }

    Database database;
    database.insert_admin(username, password);

    fmt::print("管理员账户添加成功！\n");
    LOG_INFO(fmt::format("新增管理员账户：{}", username));
}
