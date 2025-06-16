#pragma once
#include <string>
#include <Dadabase.hpp>
#include <utility>
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