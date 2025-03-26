#include "Scene.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Game.h"
#include <iostream>
#include "Util.h"

/***********************************************函数回调区****************************************************/

// 错误回调函数
static void glfw_error_callback(int error, const char *description)
{
    std::cerr << "Error: " << description << std::endl;
}

static bool is_full_screen = false;
static int win_store_width, win_store_height;
static int win_store_xpos, win_store_ypos;

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
    else if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
    {
        if (!is_full_screen)
        {
            // 保存窗口模式下的位置和大小
            glfwGetWindowPos(window, &win_store_xpos, &win_store_ypos);
            glfwGetWindowSize(window, &win_store_width, &win_store_height);

            // 获取窗口当前所在的显示器
            int monitorCount;
            GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);
            GLFWmonitor *currentMonitor = NULL;

            for (int idx = 0; idx < monitorCount; idx++)
            {
                int mx, my, mw, mh;
                glfwGetMonitorWorkarea(monitors[idx], &mx, &my, &mw, &mh);

                if (win_store_xpos >= mx && win_store_xpos < mx + mw && win_store_ypos >= my &&
                    win_store_ypos < my + mh)
                {
                    currentMonitor = monitors[idx];
                    break;
                }
            }

            if (currentMonitor == NULL)
            {
                // 如果没找到，使用主显示器
                currentMonitor = glfwGetPrimaryMonitor();
            }

            const GLFWvidmode *mode = glfwGetVideoMode(currentMonitor);

            // 切换到全屏模式
            glfwSetWindowMonitor(window, currentMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);

            is_full_screen = true;
        }
        else
        {
            // 切换回窗口模式
            glfwSetWindowMonitor(window, NULL, win_store_xpos, win_store_ypos, win_store_width, win_store_height, 0);
            is_full_screen = false;
        }
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
    Game::getInstance().On_FrameBuffer_Size(width, height);
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

    // 打印GPU的一些信息
    PrintGPUInfo();

    // 打印顶点着色器和片段着色器的最大uniform组件数量
    QueryMaxVertexAndFragmentUniformComponents();

    // 打印默认帧缓存的信息
    QueryDefaultFramebufferInfos();

    // 需要确保OpenGL上下文已经被正确创建并绑定到当前线程。
    SetupGLDebugContext();

    // 设置键盘输入回调函数
    glfwSetKeyCallback(window, glfw_key_callback);

    // 当窗口改变时，在回调函数里面重新设置视口大小
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 隐藏光标
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // 设置鼠标移动回调
    glfwSetCursorPosCallback(window, mouse_callback);

    // 设置鼠标滚轮回调
    glfwSetScrollCallback(window, scroll_callback);

    // 定义视口的宽高，铺满整个窗口
    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);
    glViewport(0, 0, fb_width, fb_height);

    glEnable(GL_DEPTH_TEST); // 开启深度测试

    /*
     * 在 OpenGL 中，如果启用了模板测试（Stencil Test）但没有明确指定任何模板函数，OpenGL 会使用默认的模板测试设置。默认情况下，模板测试的行为如下：

     * 默认模板函数：
     * 默认的模板函数是 GL_ALWAYS，即模板测试总是通过。

     * 默认模板参考值：
     * 模板参考值（Stencil Reference Value）默认是 0。

     * 默认模板掩码：
     * 模板掩码（Stencil Mask）默认是全 1，即所有位都参与测试。

     * 默认模板操作：
     * 模板操作（Stencil Operation）在 OpenGL 中由三个操作定义：

     * glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP)，即默认情况下，无论模板测试结果如何，当前帧缓冲区中的模板值都不会改变。
     * 综合来看，如果启用了模板测试但没有指定任何模板函数，模板测试将始终通过，并且不会对模板缓冲区进行任何修改。
     * 这意味着模板测试对最终的绘制结果不会产生影响，因为它的默认行为等效于模板测试被关闭的情况。
    */
    glEnable(GL_STENCIL_TEST); // 开启模板测试

    scene.Init(fb_width, fb_height);

    return true;
}

void Game::QueryDefaultFramebufferInfos() const
{
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Framebuffer is not complete: " << status << std::endl;
    }

    // 查询红色分量的位数
    // GL_BACK_LEFT 是指默认帧缓冲区的后端左缓冲区，这通常是标准的颜色缓冲区目标。
    int redBits = 0;
    glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE,
                                          &redBits);
    std::cout << "Red buffer bit size: " << redBits << std::endl;

    // 查询绿色分量的位数
    int greenBits = 0;
    glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE,
                                          &greenBits);
    std::cout << "Green buffer bit size: " << greenBits << std::endl;

    // 查询蓝色分量的位数
    int blueBits = 0;
    glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE,
                                          &blueBits);
    std::cout << "Blue buffer bit size: " << blueBits << std::endl;

    // 查询 Alpha 分量的位数
    int alphaBits = 0;
    glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE,
                                          &alphaBits);
    std::cout << "Alpha buffer bit size: " << alphaBits << std::endl;

    int depthBits = 0;
    glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE,
                                          &depthBits);
    std::cout << "Depth buffer bit size: " << depthBits << std::endl;

    GLint stencilSize = 0;
    glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE,
                                          &stencilSize);
    std::cout << "Stencil buffer bit size: " << stencilSize << std::endl;
};

void Game::QueryMaxVertexAndFragmentUniformComponents() const
{
    // 查询顶点着色器的最大 uniform 组件数
    GLint maxVertexUniformComponents = 0;
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxVertexUniformComponents);
    std::cout << "Maximum Vertex Uniform Components: " << maxVertexUniformComponents << std::endl;

    // 查询片段着色器的最大 uniform 组件数
    GLint maxFragmentUniformComponents = 0;
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxFragmentUniformComponents);
    std::cout << "Maximum Fragment Uniform Components: " << maxFragmentUniformComponents << std::endl;
}

void Game::PrintOpenGLVersion() const
{
    // 获取 OpenGL 版本（在核心模式下有效）
    GLint majorVersion, minorVersion;
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
    std::cout << "OpenGL Version: " << majorVersion << "." << minorVersion << std::endl;
}

void Game::PrintGPUInfo() const
{
    const GLubyte *vendor = glGetString(GL_VENDOR);     // 返回厂商名称
    const GLubyte *renderer = glGetString(GL_RENDERER); // 返回渲染器名称（GPU 型号）
    const GLubyte *version = glGetString(GL_VERSION);

    std::cout << "Vendor: " << vendor << std::endl;
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL Version: " << version << std::endl;
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

    /*
     * 在MacOS上，需要设置GLFW_OPENGL_FORWARD_COMPAT为GL_TRUE，
     * 这段代码告诉GLFW在创建 OpenGL 上下文时，启用前向兼容模式。
     * 在 macOS 上，使用 OpenGL 前向兼容模式通常是推荐的做法，甚至在某些情况下是必须的。 
     * 苹果公司倾向于鼓励开发者使用最新的 OpenGL 版本和特性，并逐渐移除对旧版本的支持。
     * 如果没有启用前向兼容模式，在某些版本的 macOS 上，你可能会遇到 OpenGL 相关的问题，例如无法创建 OpenGL 上下文或遇到一些兼容性错误。
     * Apple's OpenGL implementation does not support legacy functionality in a default context since macOS 10.14.
    */
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

    /*
     * 为了清除深度缓冲区，OpenGL 提供了 glClearDepth 函数，用于设置深度缓冲区清除时所使用的深度值。
     * 这个值默认情况下是1.0，对应深度缓冲的最大深度（通常为最远的可见范围）。这样新一帧的任何片元的深度值都会小于或等于此值，从而确保正确的深度测试。
     * 这也是为什么在大多数情况下，不会看到开发者为深度缓冲设置特定的初始值，因为默认的最大深度已经能满足大多数场景的需求。
    */
    // glClearDepth(1.0);

    /*
     * 当调用 glClear(GL_STENCIL_BUFFER_BIT) 时，模板缓冲区的每个像素都会被重置为一个指定的值。
     * 这个值是由 glClearStencil 函数设置的，默认情况下，这个值是 0。
     * 如果希望在清理模板缓冲区时将其重置为其他值，而不是默认的 0，可以使用 glClearStencil 函数来修改这个值。
     * 调用 glClearStencil 时，指定的值将被用作下一次调用 glClear(GL_STENCIL_BUFFER_BIT) 时模板缓冲区的重置值。
    */
    // glClearStencil(0);

    glClear(
        GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
        GL_STENCIL_BUFFER_BIT); // 状态值应用，清理掉颜色缓冲区并设置为指定的颜色，同时也清理掉深度缓冲区、模板缓冲区

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

void Game::On_FrameBuffer_Size(int width, int height)
{
    glViewport(0, 0, width, height);

    double aspect = static_cast<double>(width) / height;
    scene.UpdateCamAspect(aspect);

    Draw();
}