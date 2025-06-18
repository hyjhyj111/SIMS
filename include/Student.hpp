#pragma once
#include <string>
#include <format>
#include <utility>
#include <fmt/core.h>
#include "Manager.hpp"

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
    friend struct fmt::formatter<Student>;
    friend class Database;
    friend class manager;
};

template<>
struct fmt::formatter<Student> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const Student& s, FormatContext& ctx) const{
        return fmt::format_to(
                ctx.out(),
                "Student(name = {:^7}, major = {:^7}, id = {:^7}, age = {:^7}, sex = {:^7})",
                s.name, s.major, s.id, s.age, s.sex == 1 ? "male" : "female");
    }
};