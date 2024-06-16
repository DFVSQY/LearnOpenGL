#include "glad/glad.h"
#include "Game.h"
#include "GLFWCallback.h"
#include <iostream>

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

    Game::getInstance().Draw();
}