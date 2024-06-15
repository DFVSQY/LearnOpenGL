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

    /*
     * glGenVertexArrays 是 OpenGL 中的一个函数，用于生成一个或多个顶点数组对象 (VAO, Vertex Array Object)。

     * VAO 是一种容器对象，它存储所有与顶点属性相关的状态。VAO 记录了顶点属性的配置以及用于绘制的顶点缓冲对象的绑定情况。
     * VAO 是一个非常方便的工具，可以帮助管理顶点属性配置。只需要配置一次 VAO，然后在绘制时绑定相应的 VAO，就能恢复之前配置好的顶点属性状态。

     * glGenVertexArrays 是生成 VAO 的关键函数，它生成一个或多个 VAO 并返回其 ID。VAO 是一个强大的工具，用于管理与顶点属性相关的状态。
     * 通过使用 VAO，可以简化顶点属性的配置过程，并在绘制时快速切换不同的顶点属性配置。这对于提高渲染性能和代码可维护性非常有帮助。
    */
    glGenVertexArrays(1, pVAO);

    /*
     * 在OpenGL中，glGenBuffers函数的作用是分配一个"Buffer Object"的名称。"Buffer Object"代表了一个可以被应用程序和GPU（图形处理单元）访问的内存块。
     * 当调用glGenBuffers时，它实际上并没有创建任何东西。它只是返回一组当前未被用作缓冲区名称的整数。实际的’对象’并不会在调用glBindBuffer之前被创建。
     * 因此，glGenBuffers实际上并不是必需的，它只是作为一个方便函数，提供一个未使用的整数。
     * 所以，如果只是创建了一组随机的整数，只要它们之间没有重叠，就可以将其作为缓冲区列表，而无需调用glGenBuffers。
    */
    glGenBuffers(1, pVBO);

    /*
     * glBindVertexArray 是 OpenGL 中的一个函数，用于绑定一个顶点数组对象 (VAO, Vertex Array Object)。
     * 绑定 VAO 是一个关键步骤，因为它使得所有后续的顶点属性配置和顶点缓冲对象 (VBO, Vertex Buffer Object) 绑定都关联到这个 VAO 上。
     * 当一个 VAO 被绑定后，所有与顶点数组相关的状态改变都会被存储在这个 VAO 中。

     * bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    */
    glBindVertexArray(*pVAO);

    /*
     * 在OpenGL中，glBindBuffer函数用于将一个缓冲区对象（Buffer Object）绑定到一个指定的缓冲区绑定点。
     * 这个函数接受两个参数：一个是目标（target），另一个是缓冲区对象的名称（buffer）。

     * 当调用glBindBuffer函数时，以下几件事情会发生：
     *  1. 如果这个缓冲区对象之前没有被初始化（也就是说，这是第一次绑定这个缓冲区对象），那么OpenGL会为其分配内存。
     *  2. 这个缓冲区对象会被设置为当前活动的缓冲区对象。这意味着，之后对指定目标的所有操作都会影响到这个缓冲区对象。
     *  3. 如果之后调用了例如glBufferData或glBufferSubData等函数，这些函数会改变当前活动的缓冲区对象的数据。

     * GL_ARRAY_BUFFER：用于顶点属性数据，如顶点坐标、法线、颜色等。
    */
    glBindBuffer(GL_ARRAY_BUFFER, *pVBO);

    /*
     * glBufferData函数用于创建并初始化一个缓冲区对象的数据存储。

     * 这个函数接受四个参数：目标（target）、大小（size）、数据（data）和用途（usage）。
     *  目标（target）：这个参数定义了缓冲区对象的类型。例如，GL_ARRAY_BUFFER表示顶点数据，GL_ELEMENT_ARRAY_BUFFER表示索引数据等。
     *  大小（size）：这个参数指定了缓冲区对象的大小，以字节为单位。
     *  数据（data）：这个参数是一个指向希望存储在缓冲区对象中的数据的指针。如果这个参数不为NULL，那么缓冲区对象将会被初始化为这个指针指向的数据。
     *  用途（usage）：这个参数给出了缓冲区对象的预期使用模式。例如，GL_STATIC_DRAW表示数据将会被修改一次，但被读取多次。

     * 当调用glBufferData函数时，以下几件事情会发生：
     *  1. 如果之前已经存在一个数据存储，那么这个数据存储会被删除。
     *  2. 一个新的数据存储会被创建，大小和用途由参数指定。
     *  3. 如果数据参数不为NULL，那么新的数据存储会被初始化为这个参数指向的数据。
     *  4. glBufferData函数会创建一个数据在“快速内存”（即GPU可以直接访问的RAM）中的工作副本，而不是每次调用glDrawElements或glDrawArrays时都需要在内存域（即CPU→GPU）之间传输顶点属性。
    */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /*
     * 在OpenGL中,glVertexAttribPointer函数是用于设置顶点属性数据的重要函数。
     * 它的作用是将顶点数据(如位置、颜色、法线等)与指定的属性位置关联起来,以便GPU能够正确读取和处理这些数据。
     * 这个函数通常与顶点缓冲对象 (VBO) 一起使用，用于设置顶点着色器中顶点属性的数据来源。

     * 这个函数接受六个参数：索引（index）、大小（size）、类型（type）、是否标准化（normalized）、步长（stride）和指针（pointer）。
     *  index：指定要修改的顶点属性的索引。在顶点着色器中，这个索引与location是通过 layout (location = x) 指定的。例如，如果顶点着色器中有 layout (location = 0) in vec3 position;，那么这个索引就是 0。
     *  size：指定每个顶点属性的分量数量。例如，位置通常有3个分量（x, y, z），颜色可能有4个分量（r, g, b, a）。
     *  type：指定数组中每个组件的数据类型。常用的类型有GL_FLOAT、GL_INT、GL_UNSIGNED_INT等。
     *  normalized：如果设置为 GL_TRUE，整数类型的数值在被访问时会被映射到 [0,1] (无符号) 或 [-1,1] (有符号) 范围内。如果设置为 GL_FALSE，则直接作为浮点数处理。
     *  stride：指定连续顶点属性之间的字节偏移量。如果为 0，则表示顶点属性是紧密排列的（即无间隔）。这个参数用于在顶点缓冲对象中正确定位每个顶点的属性。
     *  pointer：指定顶点属性数组中第一个组件的字节偏移量。这个指针是从缓冲对象数据的起始位置开始计算的。
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    /*
     * 在现代 OpenGL 中，顶点数据通常通过顶点缓冲对象 (VBO) 传递到 GPU，而这些数据可以包含多个不同的属性（如位置、颜色、法线、纹理坐标等）。
     * 然而，为了使这些数据在绘制时被使用，必须启用相应的顶点属性数组。这就是 glEnableVertexAttribArray 的作用。

     * 调用这个函数后,当使用glDrawArrays或glDrawElements进行渲染时,对应位置的顶点属性数据将被GPU读取和处理,用于顶点着色器的计算。
     * 如果没有调用glEnableVertexAttribArray,即使我们使用glVertexAttribPointer设置了顶点属性数据的存储方式,GPU也不会读取和使用这些数据。
     * 需要注意的是,在设置完顶点属性指针后,默认情况下所有顶点属性数组都是禁用的,必须手动调用glEnableVertexAttribArray来启用需要使用的属性。
    */
    glEnableVertexAttribArray(0);

    /*
     * note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object,
     * so afterwards we can safely unbind
    */
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*
     * You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens.
     * Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    */
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

    /*
     * glDrawArrays 是 OpenGL 中的一个函数，用于绘制图元（如点、线、三角形等）。
     * 它根据当前绑定的顶点数组对象 (VAO) 和顶点缓冲对象 (VBO) 中的数据，按照指定的图元类型和顶点数量进行绘制。

     * 有三个参数：mode, first, count：
     *  mode：指定要绘制的图元类型。GL_TRIANGLES：绘制独立的三角形。
     *  first：指定第一个顶点的索引。它表示从哪个位置开始读取顶点数据。
     *  count：指定顶点数量。

     * glDrawArrays 的工作流程：
     *  1. 绑定 VAO：确保当前绑定的 VAO 包含所需的顶点属性配置和 VBO 绑定。
     *  2. 读取顶点数据：从绑定的 VBO 中读取顶点数据，从索引 first 开始，读取 count 个顶点。
     *  3. 组装图元：根据 mode 参数指定的图元类型，将读取的顶点数据组装成相应的图元（如点、线、三角形等）。
     *  4. 绘制图元：发送组装好的图元到 GPU 进行渲染。
    */
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // 交换缓冲区，显示图像
    glfwSwapBuffers(window);
}