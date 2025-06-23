#pragma once
class manager {
public:
    static void menu();
    static void add_student();
    static void show_student();
    static void delete_student();
    static void edit_student();
    static void find_student();
    static void show_sorted();
    static void add_admin();
    static void log_out(bool &, const std::string &);
};
