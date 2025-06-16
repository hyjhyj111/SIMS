#pragma once
#include <string>
#include <format>
#include <utility>
class Student {
public:
    Student() = default;
    Student(std::string  name, std::string  major, int id, int age, int sex)
            : name(std::move(name)), major(std::move(major)), id(id), age(age), sex(sex) {}

private:
    std::string name;
    std::string major;
    int id{};
    int age{};
    int sex{};
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