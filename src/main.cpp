#include "GLFW/glfw3.h"

int main()
{
    GLFWwindow *window = nullptr;

    // 初始化GLFW
    if (!glfwInit())
    {
        return -1;
    }

    // 创建window窗口以及OpenGL上下文
    window = glfwCreateWindow(640, 480, "LearnOpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // 设置为当前上下文
    glfwMakeContextCurrent(window);

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