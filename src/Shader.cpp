#include <iostream>
#include "Shader.h"
#include <fstream>
#include <sstream>

Shader::Shader() : shader_program(0)
{
}

bool Shader::Init(const char *vertexPath, const char *fragmentPath)
{
    std::string vertex_str = ReadShaderFile(vertexPath);
    std::string fragment_str = ReadShaderFile(fragmentPath);

    const char *vertex_content = vertex_str.c_str();
    const char *fragment_content = fragment_str.c_str();

    GLuint vertex_shader = Compile(GL_VERTEX_SHADER, vertex_content);
    if (vertex_shader == 0)
    {
        std::cerr << "Shader Init failed, vertex shader compile failed!" << std::endl;
        return false;
    }

    GLuint fragment_shader = Compile(GL_FRAGMENT_SHADER, fragment_content);
    if (fragment_shader == 0)
    {
        std::cerr << "Shader Init failed, fragment shader compile failed!" << std::endl;
        return false;
    }

    shader_program = Link(vertex_shader, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return true;
}

void Shader::Use()
{
    /*
     * 主要作用:
     *  激活指定的着色器程序,使其成为当前使用的程序。
     *  后续的渲染调用将使用这个程序来处理顶点和片段。

     * 状态改变:
     *  调用 glUseProgram 会改变 OpenGL 的状态机,将指定的程序设置为当前活动程序。
     *  这个状态会一直保持,直到调用 glUseProgram 使用另一个程序或传入 0 来禁用着色器程序。

     * 使用时机:
     *  通常在渲染循环中,在绘制需要使用特定着色器的对象之前调用。
     *  可以根据需要多次切换不同的程序。

     * 与 uniform 变量的关系:
     *  只有在程序被激活后,才能设置其 uniform 变量的值。
    */
    glUseProgram(shader_program);
}

// 向shader传递bool值
void Shader::SetBool(const std::string &name, bool value) const
{
    GLint localtion = GetUniformLocation(name);
    glUniform1i(localtion, (GLint)value);
}

// 向shader传递int值
void Shader::SetInt(const std::string &name, GLint value) const
{
    GLint localtion = GetUniformLocation(name);
    glUniform1i(localtion, value);
}

// 向shader传递float值
void Shader::SetFloat(const std::string &name, GLfloat value) const
{
    GLint localtion = GetUniformLocation(name);

    glUniform1f(localtion, value);
}

// 向shader传递浮点型vec4值
void Shader::SetFloat4(const std::string &name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const
{
    GLint localtion = GetUniformLocation(name);

    /*
     * glUniform4f函数用于设置uniform变量的值，特别是那些类型为四元素浮点向量（vec4）的uniform变量。
     * 需要先调用glUseProgram，因为它会在当前活动的着色器程序上设置统一变量。

     * 函数原型：GLint glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
     *  location：uniform变量的位置索引，这个位置是通过glGetUniformLocation函数获取的。
     *  v0到v3：分别是四元素向量的x、y、z和w分量的值。
    */
    glUniform4f(localtion, v0, v1, v2, v3);
}

// 获取uniform变量位置
GLint Shader::GetUniformLocation(const std::string &name) const
{
    /*
     * glGetUniformLocation函数用于查询特定uniform变量在给定着色器程序中的位置（位置索引）。
     * 每个uniform变量在编译和链接着色器程序后都会被分配一个位置，但这个位置不是由开发者指定的，而是由OpenGL决定的。
     * 为了设置一个uniform变量的值，你需要先知道这个变量在着色器程序中的位置。

     * 函数原型：GLint glGetUniformLocation(GLuint program, const GLchar *name);
     *  program：已链接的着色器程序的ID。
     *  name：你想要查询位置的uniform变量的名称，这个名称应与着色器代码中声明的名称匹配。
     *  返回值：如果查询成功，返回uniform变量的位置索引；如果查询失败（例如，如果变量不存在或者没有被着色器程序使用），返回-1。
    */
    GLint location = glGetUniformLocation(shader_program, name.c_str());
    if (location == -1)
    {
        std::cerr << "Uniform variable '" << name << "' doesn't exist!" << std::endl;
    }
    return location;
}

GLuint Shader::Compile(GLenum shaderType, const char *fileContent)
{
    GLenum shader_type = shaderType;
    const char *file_content = fileContent;

    /*
     * glCreateShader 函数创建一个指定类型的着色器对象，并返回其标识符。
     * 这个对象将用于存储和编译 GLSL 代码。
    */
    GLuint shader = glCreateShader(shader_type);

    /*
     * glShaderSource 函数将 GLSL 着色器源码上传到指定的着色器对象中。这些源码将在后续步骤中被编译成可执行的着色器程序。

     * 函数原型：void glShaderSource(GLuint shader, GLsizei count, const GLchar *const* string, const GLint *length);
     *  shader：着色器对象的标识符，由 glCreateShader 函数返回。
     *  count：字符串数组中的字符串数量。
     *  string：指向着色器源码字符串数组的指针。
     *  length：每个字符串的长度。如果是 NULL，则认为每个字符串都以 ‘\0’ 结尾。
    */
    glShaderSource(shader, 1, &file_content, NULL);

    /*
     * glCompileShader 函数编译之前通过 glShaderSource 上传的着色器源码。如果源码有错误，编译将失败，可以通过查询编译状态来获取错误信息。

     * 函数原型：void glCompileShader(GLuint shader);
     *  shader：着色器对象的标识符，由 glCreateShader 函数返回。
    */
    glCompileShader(shader);

    // check for shader compile errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "error, shader compilation is failed:\n" << infoLog << "\n" << std::endl;
    }

    return shader;
}

GLuint Shader::Link(GLuint vertexShader, GLuint fragmentShader)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // check for linking errors
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "error, link shader program failed\n" << infoLog << std::endl;
    }

    return program;
}

std::string Shader::ReadShaderFile(const char *filePath)
{
    std::string str_content;
    std::ifstream shader_file;

    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        shader_file.open(filePath);
        if (!shader_file.is_open())
        {
            throw std::ios_base::failure("Failed to open the file.");
        }

        std::stringstream shader_stream;
        shader_stream << shader_file.rdbuf();
        shader_file.close();

        str_content = shader_stream.str();
    }
    catch (const std::ifstream::failure &e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR::SHADER::GENERAL_EXCEPTION: " << e.what() << std::endl;
    }

    return str_content;
}

Shader::~Shader()
{
}