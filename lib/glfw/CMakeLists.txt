project(GLFWLib C)

# 将 src 目录下的所有 .c 文件添加到变量 SRC
file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "src/*.c")

# 添加库
add_library(glfw ${SRC})

if(WIN32)
    target_compile_definitions(glfw PRIVATE _GLFW_WIN32)
    target_compile_definitions(glfw PRIVATE UNICODE _UNICODE)
endif()

if(APPLE)
    target_compile_definitions(glfw PRIVATE _GLFW_COCOA)

    target_sources(glfw PRIVATE src/cocoa_platform.h src/cocoa_joystick.h src/cocoa_init.m
        src/cocoa_joystick.m src/cocoa_monitor.m src/cocoa_window.m
        src/nsgl_context.m)

    target_link_libraries(glfw PRIVATE "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreFoundation")
endif()

# 添加包含目录
target_include_directories(glfw PUBLIC "include")