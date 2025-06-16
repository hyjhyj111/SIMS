#include <Dadabase.hpp>
Database::Database() {
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql, "localhost", "root", "123456", "SIMS", 3306, nullptr, 0))
    {
        throw std::runtime_error(std::format("数据库连接失败 {}", mysql_errno(&mysql)));
    }
}

bool Database::admin_exit(const std::string &username, const std::string &password) {
    std::string sql = "SELECT * FROM admin WHERE username='" + username + "' AND password='" + password + "';";
    Result result = _query(sql);
    return !result.rows.empty();
}

bool Database::id_exit(int id) {
    std::string sql = std::format("SELECT * FROM student WHERE id='{}'", id);
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
        throw std::runtime_error(std::format("SQL 查询失败: {}", mysql_errno(&mysql)));
    }

    MYSQL_RES* result = mysql_store_result(&mysql);
    if (!result) {
        throw std::runtime_error("mysql_store_result 失败或没有返回结果集");
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
    return qr;
}

void Database::_insert(const std::string &sql) {
    if (mysql_query(&mysql, sql.c_str()) != 0) {
        throw std::runtime_error(std::format("执行失败: {}", mysql_errno(&mysql)));
    }
}
Student Database::select_by_id(int id) {
    std::string sql = "SELECT name, major, id, age, sex FROM student WHERE id = " + std::to_string(id) + ";";
    Result result = _query(sql);

    if (result.rows.empty()) {
        throw std::runtime_error("找不到对应的学生 ID");
    }

    const auto& row = result.rows[0];
    if (row.size() != 5) {
        throw std::runtime_error("数据库返回字段数量不匹配");
    }

    return {row[0], row[1], std::stoi(row[2]), std::stoi(row[3]), std::stoi(row[4])};
}
