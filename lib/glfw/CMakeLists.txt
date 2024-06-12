# 将 src 目录下的所有 .c 文件添加到变量 SRC
file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "src/*.c")

# 添加库
add_library(glfw ${SRC})

# 添加包含目录
target_include_directories(glfw PUBLIC "include")

set_target_properties(glfw PROPERTIES LINKER_LANGUAGE CXX)