#pragma once
#include <string>
#include <format>
class Student {
public:
    Student() = default;
    Student(const std::string& name, const std::string& major, int id, int age, int sex)
            : name(name), major(major), id(id), age(age), sex(sex) {}

private:
    std::string name;
    std::string major;
    int id;
    int age;
    int sex;
    friend struct std::formatter<Student>;
    friend class Database;
};

template <>
struct std::formatter<Student> : std::formatter<std::string> {
    auto format(const Student& s, auto& ctx) {
        return formatter<std::string>::format(
                std::format("Student(name: {}, major: {}, id: {}, age: {}, sex: {})",
                            s.name, s.major, s.id, s.age, s.sex),
                ctx);
    }
};