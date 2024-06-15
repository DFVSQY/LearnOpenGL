#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "Shader.h"

Shader::Shader() : shader_program(0), vertex_file_path(nullptr), fragment_file_path(nullptr)
{
}

GLuint Shader::Init(const char *vertexFilePath, const char *fragmentFilePath)
{
    vertex_file_path = vertexFilePath;
    fragment_file_path = fragmentFilePath;

    if (vertex_file_path == nullptr || vertex_file_path[0] == '\0')
    {
        std::cerr << "vertex file path is null or empty!" << std::endl;
        return 0;
    }

    if (fragment_file_path == nullptr || fragment_file_path[0] == '\0')
    {
        std::cerr << "fragment file path is null or empty!" << std::endl;
        return 0;
    }

    GLuint vertex_shader = Compile(GL_VERTEX_SHADER);
    if (vertex_shader == 0)
    {
        std::cerr << "Init failed, vertex shader compile failed!" << std::endl;
        return 0;
    }

    GLuint fragment_shader = Compile(GL_FRAGMENT_SHADER);
    if (fragment_shader == 0)
    {
        std::cerr << "Init failed, fragment shader compile failed!" << std::endl;
        return 0;
    }

    shader_program = Link(vertex_shader, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

GLuint Shader::GetShaderProgram() const
{
    return shader_program;
}

GLuint Shader::Compile(GLenum shaderType)
{
    GLenum shader_type = shaderType;

    const char *file_path = nullptr;
    if (shader_type == GL_VERTEX_SHADER)
    {
        file_path = vertex_file_path;
    }
    else if (shader_type == GL_FRAGMENT_SHADER)
    {
        file_path = fragment_file_path;
    }

    if (file_path == nullptr || file_path[0] == '\0')
    {
        std::cerr << "file path is null!" << std::endl;
        return 0;
    }

    const char *file_content = ReadShaderContent(file_path);
    if (file_content == nullptr || file_content[0] == '\0')
    {
        std::cerr << "file content is null!" << std::endl;
        return 0;
    }

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

    // 释放文件内容存储
    free((void *)file_content);

    // check for shader compile errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "error, shader compilation is failed:\n"
                  << infoLog << "\n"
                  << "file path:" << file_path << std::endl;
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
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    return program;
}

const char *Shader::ReadShaderContent(const char *filePath)
{
    const char *file_path = filePath;

    if (file_path == nullptr)
    {
        std::cerr << "file path is null!" << std::endl;
        return nullptr;
    }

    FILE *file = nullptr;
    errno_t err = fopen_s(&file, file_path, "rb");
    if (err != 0 || !file)
    {
        std::cerr << "Failed to open shader file: " << file_path << std::endl;
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

Shader::~Shader()
{
}