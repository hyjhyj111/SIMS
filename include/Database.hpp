#pragma once
#include <mysql/mysql.h>
#include <vector>
#include <string>
#include <Student.hpp>
struct Result {
    std::vector<std::string> col_name;
    std::vector<std::vector<std::string>> rows;
};
class Database{
public:
    Database() ;
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    bool admin_exit(const std::string& username, const std::string& password) ;
    bool id_exit(int id);
    std::vector<Student> students() ;
    void insert_student(const Student &) ;
    void insert_admin(const std::string&, const std::string&) ;
    Student select_by_id(int );
    bool delete_by_id(int id);
    ~Database() ;
private:
    MYSQL mysql{};
    Result _query(const std::string& sql) ;
    void _insert(const std::string& sql) ;


};