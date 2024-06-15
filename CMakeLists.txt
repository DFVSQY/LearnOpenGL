cmake_minimum_required(VERSION 3.27)

# 设置项目名称
project(LearnOpenGL CXX)

# 生成 compile_commands.json 文件
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置C++标准为C++20
set(CMAKE_CXX_STANDARD 20)

# 要求必须使用这个版本的标准
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall)

add_subdirectory(lib/glfw)
add_subdirectory(lib/glad)

# 将 src 目录下的所有 .cpp 文件添加到变量 SRC 中
file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "src/*.cpp")

# 添加可执行文件
add_executable(main ${SRC})

# 指定头文件搜索目录
target_include_directories(main PRIVATE "includes")

# 链接 glfw, glad 库
target_link_libraries(main PRIVATE glfw glad)

if(WIN32)
    target_link_libraries(main PRIVATE opengl32)
endif()