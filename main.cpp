#include <bits/stdc++.h>
#include <Dadabase.hpp>
#include <Admin.hpp>
void solve() {
    Database database;
    Student student("12", "12", 1, 2, 0);
    database.insert_student(student);
}
int main() {
    try {
        solve();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
