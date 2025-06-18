cmake_minimum_required(VERSION 3.14)
project(stu)
if(APPLE)
    set(CMAKE_CXX_STANDARD 23)

    # 自动查找源文件
    file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
    add_executable(stu ${SOURCES})

    # 包含头文件路径
    target_include_directories(stu PRIVATE include)

    # 使用 fmt 官方 CMake 配置
    add_subdirectory(dependencies/fmt EXCLUDE_FROM_ALL)

    # MySQL and fmt 配置
    target_include_directories(stu PRIVATE /opt/homebrew/include)
    target_link_directories(stu PRIVATE /opt/homebrew/lib)
    target_link_libraries(stu PRIVATE mysqlclient fmt-header-only)
elseif(UNIX)
    set(CMAKE_CXX_STANDARD 23)

    # 自动查找源文件
    file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
    add_executable(stu ${SOURCES})

    # 包含头文件路径
    target_include_directories(stu PRIVATE include)

    # 使用 fmt 官方 CMake 配置
    add_subdirectory(dependencies/fmt EXCLUDE_FROM_ALL)

    # MySQL and fmt 配置
    target_include_directories(stu PRIVATE /usr/include)
    target_link_libraries(stu PRIVATE mysqlclient fmt::fmt-header-only)
endif ()