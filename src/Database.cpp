#include <fmt/format.h>
#include <Database.hpp>
#include <Logger.hpp>
Database::Database() {
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql, "localhost", "root", "123456", "SIMS", 3306, nullptr, 0))
    {
        LOG_ERROR(fmt::format("数据库连接失败 {}", mysql_errno(&mysql)));
    }
    LOG_INFO("The database initialization has been completed");
}

bool Database::admin_exit(const std::string &username, const std::string &password) {
    std::string sql = "SELECT * FROM admin WHERE username='" + username + "' AND password='" + password + "';";
    Result result = _query(sql);
    return !result.rows.empty();
}

bool Database::id_exit(int id) {
    std::string sql = fmt::format("SELECT * FROM student WHERE id='{}'", id);
    Result result = _query(sql);
    return !result.rows.empty();
}


Database::~Database() {
    mysql_close(&mysql);
}

std::vector<Student> Database::students() {
    Result result = _query("SELECT name, major, id, age, sex FROM student");
    std::vector<Student> students;
    for (const auto& row : result.rows) {

        students.emplace_back(
                row[0],                         // name
                row[1],                         // major
                std::stoi(row[2]),              // id
                std::stoi(row[3]),              // age
                std::stoi(row[4])               // sex
        );
    }
    return students;
}

void Database::insert_student(const Student& s) {
    std::string sql = "INSERT INTO student (name, major, id, age, sex) VALUES ('" +
                      s.name + "', '" + s.major + "', " +
                      std::to_string(s.id) + ", " +
                      std::to_string(s.age) + ", " +
                      std::to_string(s.sex) + ");";
    _insert(sql);
}

void Database::insert_admin(const std::string &username, const std::string &password) {
    std::string sql = "INSERT INTO admin (username, password) VALUES ('" +
                      username + "', '" + password + "');";
    _insert(sql);
}

Result Database::_query(const std::string &sql) {
    if (mysql_query(&mysql, sql.c_str()) != 0) {
        LOG_ERROR(fmt::format("SQL 查询失败: {}", mysql_errno(&mysql)));
        exit(1);
    }

    MYSQL_RES* result = mysql_store_result(&mysql);
    if (!result) {
        LOG_WARNING("mysql_store_result 失败或没有返回结果集");
        return {};
    }

    unsigned int num_fields = mysql_num_fields(result);
    MYSQL_FIELD* fields = mysql_fetch_fields(result);

    Result qr;
    for (int i = 0; i < num_fields; ++i) {
        qr.col_name.emplace_back(fields[i].name);
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        std::vector<std::string> row_data;
        for (int i = 0; i < num_fields; ++i) {
            row_data.emplace_back(row[i]);
        }
        qr.rows.push_back(std::move(row_data));
    }

    mysql_free_result(result);
    LOG_INFO("Query completed");
    return qr;
}

void Database::_insert(const std::string &sql) {
    if (mysql_query(&mysql, sql.c_str()) != 0) {
        LOG_ERROR(fmt::format("执行失败: {}", mysql_errno(&mysql)));
        exit(1);
    }
    LOG_INFO("Insert complete");
}
Student Database::select_by_id(int id) {
    std::string sql = "SELECT name, major, id, age, sex FROM student WHERE id = " + std::to_string(id) + ";";
    Result result = _query(sql);

    if (result.rows.empty()) {
        LOG_ERROR("找不到对应的学生 ID");
        exit(1);
    }

    const auto& row = result.rows[0];
    if (row.size() != 5) {
        LOG_ERROR("数据库返回字段数量不匹配");
        exit(1);
    }

    return {row[0], row[1], std::stoi(row[2]), std::stoi(row[3]), std::stoi(row[4])};
}

bool Database::delete_by_id(int id) {
    std::string sql = fmt::format("DELETE FROM student WHERE id = {}", id);
    if (mysql_query(&mysql, sql.c_str()) != 0) {
        LOG_ERROR(fmt::format("删除失败: {}", mysql_error(&mysql)));
        return false;
    }
    LOG_INFO(fmt::format("成功删除 ID 为 {} 的学生", id));
    return mysql_affected_rows(&mysql) > 0;
}