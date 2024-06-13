#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <cstdio>
#include <gl/GL.h>

// 错误回调函数
void glfw_error_callback(int error, const char *description)
{
    printf("Error: %s\n", description);
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
}

int main()
{
    // 设置错误回调函数
    glfwSetErrorCallback(glfw_error_callback);

    // 初始化GLFW
    if (!glfwInit())
    {
        return -1;
    }

    // 创建窗口前设置一些窗口选项值（使用OpenGL3.3版本，并且使用核心配置模式）
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 定义窗口的宽高
    int win_width = 800, win_height = 600;

    // 创建window窗口以及OpenGL上下文
    GLFWwindow *window = glfwCreateWindow(win_width, win_height, "LearnOpenGL", NULL, NULL);
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

    // 设置键盘输入回调函数
    glfwSetKeyCallback(window, glfw_key_callback);

    // 当窗口改变时，在回调函数里面重新设置视口大小
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 定义视口的宽高，铺满整个窗口
    glViewport(0, 0, win_width, win_height);

    while (!glfwWindowShouldClose(window))
    {
        // 清理缓冲区并设置为指定的颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 状态值设置，用于指定颜色值
        glClear(GL_COLOR_BUFFER_BIT);         // 状态值应用，清理掉颜色缓冲区并设置为指定的颜色

        // 交换缓冲区，显示图像
        glfwSwapBuffers(window);

        // 检测事件，如键盘输入，鼠标移动等
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}