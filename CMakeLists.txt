cmake_minimum_required(VERSION 3.16)

project(InterceptionTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 寻找 Qt 6 组件
find_package(Qt6 REQUIRED COMPONENTS Core)

add_executable(InterceptionTest main.cpp
    interception.h)

# 链接 Qt 库
target_link_libraries(InterceptionTest PRIVATE Qt6::Core)

# 链接 Interception 库 (请确保 interception.lib 在当前目录)
target_link_libraries(InterceptionTest PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/interception.lib")

# 设置控制台输出 (Windows 特性)
set_target_properties(InterceptionTest PROPERTIES WIN32_EXECUTABLE FALSE)
