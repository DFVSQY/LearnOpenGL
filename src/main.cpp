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

int main()
{
    // 设置错误回调函数
    glfwSetErrorCallback(glfw_error_callback);

    // 初始化GLFW
    if (!glfwInit())
    {
        return -1;
    }

    // 创建window窗口以及OpenGL上下文
    GLFWwindow *window = glfwCreateWindow(640, 480, "LearnOpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // 设置为当前上下文
    glfwMakeContextCurrent(window);

    // 设置键盘输入回调函数
    glfwSetKeyCallback(window, glfw_key_callback);

    while (!glfwWindowShouldClose(window))
    {
        // 清理缓冲区
        glClear(GL_COLOR_BUFFER_BIT);

        // 交换缓冲区，显示图像
        glfwSwapBuffers(window);

        // 检测事件，如键盘输入，鼠标移动等
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}