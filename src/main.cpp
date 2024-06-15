#include "glad/glad.h"
#include "GLFW/glfw3.h"

#ifdef _WIN32
#include <gl/GL.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "Shader.h"
#include "Mesh.h"

GLFWwindow *window = nullptr;

Shader shader;

Mesh mesh;

void setup_window_hint();

void glfw_error_callback(int error, const char *description);

void glfw_key_callback(GLFWwindow *win, int key, int scancode, int action, int mods);

void framebuffer_size_callback(GLFWwindow *win, int width, int height);

void draw_content();

int main()
{
    // 设置错误回调函数
    glfwSetErrorCallback(glfw_error_callback);

    // 初始化GLFW
    if (!glfwInit())
    {
        return -1;
    }

    // 进行一些Windows Hint的设置
    setup_window_hint();

    // 定义窗口的宽高
    int win_width = 800, win_height = 600;

    // 创建window窗口以及OpenGL上下文
    window = glfwCreateWindow(win_width, win_height, "LearnOpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // 设置为当前上下文
    glfwMakeContextCurrent(window);

    // 使用GLFW库来获取OpenGL函数的地址，并使用glad库来加载这些地址到相应的函数指针中，
    // 要在绑定了当前上下文后才能调用该函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return -1;
    }

    GLuint shader_program = shader.Init("../shaders/vertex_01.glsl", "../shaders/fragment_01.glsl");
    if (shader_program == 0)
    {
        glfwTerminate();
        return -1;
    }

    // 顶点数据
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  bottom
        0.5f,  -0.5f, 0.0f, // right bottom
        0.0f,  0.5f,  0.0f  // center top
    };
    bool mesh_succ = mesh.Init(vertices, 9, &shader);
    if (!mesh_succ)
    {
        glfwTerminate();
        return -1;
    }

    // 设置键盘输入回调函数
    glfwSetKeyCallback(window, glfw_key_callback);

    // 当窗口改变时，在回调函数里面重新设置视口大小
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

// 定义视口的宽高，铺满整个窗口
#ifdef __APPLE__
    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);
    glViewport(0, 0, fb_width, fb_height);
#else
    glViewport(0, 0, win_width, win_height);
#endif

    // glfwSetWindowSize(window, win_width, win_height);

    while (!glfwWindowShouldClose(window))
    {
        // 绘制渲染内容
        draw_content();

        // 检测事件，如键盘输入，鼠标移动等
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

void setup_window_hint()
{
    // 创建窗口前设置一些窗口选项值（使用OpenGL3.3版本，并且使用核心配置模式）
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

// 错误回调函数
void glfw_error_callback(int error, const char *description)
{
    std::cerr << "Error: " << description << std::endl;
}

// 键盘输入回调函数
void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);

    draw_content();
}

void draw_content()
{
    // 清理缓冲区并设置为指定的颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 状态值设置，用于指定颜色值
    glClear(GL_COLOR_BUFFER_BIT);         // 状态值应用，清理掉颜色缓冲区并设置为指定的颜色

    // draw our first triangle
    mesh.Draw();

    // 交换缓冲区，显示图像
    glfwSwapBuffers(window);
}