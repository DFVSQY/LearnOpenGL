#include "Scene.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Game.h"
#include <iostream>

/***********************************************函数回调区****************************************************/

// 错误回调函数
static void glfw_error_callback(int error, const char *description)
{
    std::cerr << "Error: " << description << std::endl;
}

// 键盘输入回调函数
static void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        Game::getInstance().On_Key_W_Press();
    }
    else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        Game::getInstance().On_Key_A_Press();
    }
    else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        Game::getInstance().On_Key_S_Press();
    }
    else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        Game::getInstance().On_Key_D_Press();
    }
}

static void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    Game::getInstance().On_Mouse_Move(xpos, ypos);
}

static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    Game::getInstance().On_Mouse_Scroll(xoffset, yoffset);
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);

    Game::getInstance().Draw();
}

static void APIENTRY glfw_debug_output(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
                                       const char *message, const void *userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        std::cout << "Source: API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        std::cout << "Source: Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        std::cout << "Source: Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        std::cout << "Source: Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        std::cout << "Source: Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        std::cout << "Source: Other";
        break;
    }
    std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        std::cout << "Type: Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cout << "Type: Deprecated Behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cout << "Type: Undefined Behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::cout << "Type: Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::cout << "Type: Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        std::cout << "Type: Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        std::cout << "Type: Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        std::cout << "Type: Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        std::cout << "Type: Other";
        break;
    }
    std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << "Severity: high";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << "Severity: medium";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << "Severity: low";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        std::cout << "Severity: notification";
        break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}
/***********************************************************************************************************/

Game::Game() : window(nullptr), scene() {};

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

    // 需要确保OpenGL上下文已经被正确创建并绑定到当前线程。
    SetupGLDebugContext();

    // 设置键盘输入回调函数
    glfwSetKeyCallback(window, glfw_key_callback);

    // 当窗口改变时，在回调函数里面重新设置视口大小
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 隐藏光标
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // 设置鼠标移动回调
    glfwSetCursorPosCallback(window, mouse_callback);

    // 设置鼠标滚轮回调
    glfwSetScrollCallback(window, scroll_callback);

    // 定义视口的宽高，铺满整个窗口
    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);
    glViewport(0, 0, fb_width, fb_height);

    glEnable(GL_DEPTH_TEST); // 开启深度测试

    scene.Init(fb_width, fb_height);

    return true;
}

void Game::PrintOpenGLVersion() const
{
    // 获取 OpenGL 版本（在核心模式下有效）
    GLint majorVersion, minorVersion;
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
    std::cout << "OpenGL Version: " << majorVersion << "." << minorVersion << std::endl;
}

void Game::SetupWindowHint() const
{
    // 创建窗口前设置一些窗口选项值（使用OpenGL3.3版本，并且使用核心配置模式）
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);

#if __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // MacOS也只能支持到OpenGL4.1了
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /*
     * 启用OpenGL的调试上下文，在调试上下文中使用OpenGL可能会比非调试上下文显著变慢，
     * 因此在进行优化或发布应用程序时，需要移除GLFW的调试请求提示。
     * OpenGL4.3及以上版本支持。
    */
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
}

void Game::SetupGLDebugContext() const
{
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    int result = flags & GL_CONTEXT_FLAG_DEBUG_BIT;
    if (!result)
    {
        std::cerr << "Failed to enable OpenGL debug context" << std::endl;
    }
    else
    {
        /*
         * 启用OpenGL的调试输出功能。
         * 这允许OpenGL生成调试消息，这些消息可以帮助开发者识别错误、性能问题和其他重要信息。
        */
        glEnable(GL_DEBUG_OUTPUT);

        /*
         * 启用同步调试输出。
         * 当启用时，调试消息会在导致该消息的OpenGL命令执行完毕后立即生成。
         * 这对于调试非常有用，因为它可以确保调试消息与导致它的OpenGL调用直接关联。
        */
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        /*
         * 设置调试消息回调函数。
         * 第一个参数 是一个用户定义的函数，它会在OpenGL生成调试消息时被调用。
         * 第二个参数 是一个可选的用户指定数据指针，可以传递给回调函数。
        */
        glDebugMessageCallback(glfw_debug_output, nullptr);

        /*
         * 控制哪些类型的调试消息应该被生成。
         * 配置系统接收所有类型的调试消息，不进行任何过滤。
        */
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
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
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT); // 状态值应用，清理掉颜色缓冲区并设置为指定的颜色，同时也清理掉深度缓冲区

    scene.Render();

    // 交换缓冲区，将渲染结果显示到窗口中
    glfwSwapBuffers(window);
}

void Game::On_Key_W_Press()
{
    scene.MoveCamForward();
}

void Game::On_Key_S_Press()
{
    scene.MoveCamBackward();
}

void Game::On_Key_A_Press()
{
    scene.MoveCamLeft();
}

void Game::On_Key_D_Press()
{
    scene.MoveCamRight();
}

void Game::On_Mouse_Move(double xpos, double ypos)
{
    scene.UpdateCamYawAndPitch(xpos, ypos);
}

void Game::On_Mouse_Scroll(double xoffset, double yoffset)
{
    scene.UpdateCamZoom(yoffset);
}