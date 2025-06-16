cmake_minimum_required(VERSION 3.31)
project(stu)

set(CMAKE_CXX_STANDARD 23)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/main.cpp)
add_executable(stu src/main.cpp ${SOURCES})
target_include_directories(stu PRIVATE include)
# 设置 MySQL 头文件路径
# 正确写法
target_include_directories(stu PRIVATE /opt/homebrew/include)

# 设置 MySQL 链接库路径
target_link_directories(stu PRIVATE /opt/homebrew/lib)

# 链接 mysqlclient 库
target_link_libraries(stu PRIVATE mysqlclient)