#include "Scene.h"
#include "Shader.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "GLFWCallback.h"
#include "Game.h"
#include <iostream>

Game::Game() : window(nullptr), scene(){};

Game::~Game()
{
    if (!window)
    {
        glfwDestroyWindow(window);
        window = nullptr;

        glfwTerminate();
    }
}

Game &Game::getInstance()
{
    static Game instance; // Guaranteed to be destroyed.
                          // Instantiated on first use.
    return instance;
}

bool Game::Init(const char *title, int width, int height)
{
    // 设置错误回调函数
    glfwSetErrorCallback(glfw_error_callback);

    // 初始化GLFW
    if (!glfwInit())
        return false;

    SetupWindowHint();

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return false;
    }

    // 设置为当前上下文
    glfwMakeContextCurrent(window);

    // 使用GLFW库来获取OpenGL函数的地址，并使用glad库来加载这些地址到相应的函数指针中，
    // 要在绑定了当前上下文后才能调用该函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return false;
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

    scene.Init();

    return true;
}

void Game::SetupWindowHint()
{
    // 创建窗口前设置一些窗口选项值（使用OpenGL3.3版本，并且使用核心配置模式）
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void Game::Run()
{
    while (!glfwWindowShouldClose(window))
    {
        // 渲染
        Draw();

        // 处理事件
        glfwPollEvents();
    }
}

void Game::Draw()
{
    // 清理缓冲区并设置为指定的颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 状态值设置，用于指定颜色值
    glClear(GL_COLOR_BUFFER_BIT);         // 状态值应用，清理掉颜色缓冲区并设置为指定的颜色

    scene.Render();

    // 交换缓冲区，将渲染结果显示到窗口中
    glfwSwapBuffers(window);
}