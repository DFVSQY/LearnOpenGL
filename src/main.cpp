#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <gl/GL.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

GLFWwindow *window = nullptr;
GLuint shaderProgram;
GLuint VAO, VBO;

void setup_window_hint();

void glfw_error_callback(int error, const char *description);

void glfw_key_callback(GLFWwindow *win, int key, int scancode, int action, int mods);

void framebuffer_size_callback(GLFWwindow *win, int width, int height);

const char *read_shader_content(const char *path);

void free_shader_content(const char *shader_source);

int create_shader_program(GLuint *shaderProgram);

void setup_vertex_data(GLuint *VAO, GLuint *VBO);

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
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    if (create_shader_program(&shaderProgram) != 0)
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    setup_vertex_data(&VAO, &VBO);

    // 设置键盘输入回调函数
    glfwSetKeyCallback(window, glfw_key_callback);

    // 当窗口改变时，在回调函数里面重新设置视口大小
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 定义视口的宽高，铺满整个窗口
    glViewport(0, 0, win_width, win_height);

    while (!glfwWindowShouldClose(window))
    {
        // 绘制渲染内容
        draw_content();

        // 检测事件，如键盘输入，鼠标移动等
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void setup_window_hint()
{
    // 创建窗口前设置一些窗口选项值（使用OpenGL3.3版本，并且使用核心配置模式）
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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

// 读取Shader文件内容
const char *read_shader_content(const char *filePath)
{
    FILE *file = nullptr;
    errno_t err = fopen_s(&file, filePath, "rb");
    if (err != 0 || !file)
    {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        return nullptr;
    }

    // 移动文件指针到文件末尾
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 分配内存保存文件内容
    char *buffer = (char *)malloc(fileSize + 1);
    if (!buffer)
    {
        std::cerr << "Failed to allocate memory." << std::endl;
        fclose(file);
        return nullptr;
    }

    // 读取文件内容到缓冲区
    size_t bytesRead = fread(buffer, 1, fileSize, file);
    if (bytesRead != fileSize)
    {
        std::cerr << "Failed to read file content." << std::endl;
        free(buffer);
        fclose(file);
        return nullptr;
    }

    // 添加字符串终止符
    buffer[fileSize] = '\0';

    fclose(file);
    return buffer;
}

void free_shader_content(const char *shaderContent)
{
    free((void *)shaderContent);
}

// 创建Shader程序
int create_shader_program(GLuint *pShader)
{
    const char *vertexShaderSource = read_shader_content("../shaders/vertex_01.glsl");
    if (!vertexShaderSource)
    {
        std::cerr << "Failed to read vertex shader file." << std::endl;
        return -1;
    }

    const char *fragmentShaderSource = read_shader_content("../shaders/fragment_01.glsl");
    if (!fragmentShaderSource)
    {
        std::cerr << "Failed to read fragment shader file." << std::endl;
        return -1;
    }

    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    *pShader = glCreateProgram();
    GLuint shaderProgram = *pShader;
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    free_shader_content(vertexShaderSource);
    free_shader_content(fragmentShaderSource);

    return 0;
}

void setup_vertex_data(GLuint *pVAO, GLuint *pVBO)
{
    // 顶点数据
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  bottom
        0.5f,  -0.5f, 0.0f, // right bottom
        0.0f,  0.5f,  0.0f  // center top
    };

    glGenVertexArrays(1, pVAO);
    glGenBuffers(1, pVBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(*pVAO);

    glBindBuffer(GL_ARRAY_BUFFER, *pVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound
    // vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens.
    // Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs)
    // when it's not directly necessary.
    glBindVertexArray(0);
}

void draw_content()
{
    // 清理缓冲区并设置为指定的颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 状态值设置，用于指定颜色值
    glClear(GL_COLOR_BUFFER_BIT);         // 状态值应用，清理掉颜色缓冲区并设置为指定的颜色

    // draw our first triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll
                            // do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // 交换缓冲区，显示图像
    glfwSwapBuffers(window);
}