#include <iostream>
#include "Shader.h"
#include <fstream>
#include <sstream>
#include "glm/gtc/type_ptr.hpp"

Shader::Shader(const ShaderUnit &vertexUnit, const ShaderUnit &fragmentUnit) : shader_program(0), texture_idx(0)
{
    const GLuint vertex_shader = vertexUnit.GetShaderID();
    if (vertex_shader == 0)
    {
        std::cerr << "Shader Init failed, vertex shader compile failed!" << std::endl;
        return;
    }

    const GLuint fragment_shader = fragmentUnit.GetShaderID();
    if (fragment_shader == 0)
    {
        std::cerr << "Shader Init failed, fragment shader compile failed!" << std::endl;
        return;
    }

    shader_program = Link(vertex_shader, fragment_shader);
}

Shader::~Shader()
{
    if (shader_program > 0)
    {
        /*
         * glDeleteProgram 是 OpenGL 中用于删除着色器程序对象的函数。
         * 它的主要作用是释放与特定着色器程序对象相关的资源，以避免内存泄漏和其他不必要的资源占用。
         * glDeleteProgram 函数会删除指定的着色器程序对象，释放其占用的 GPU 资源。

         * 函数原型：void glDeleteProgram(GLuint program);
         *  program：要删除的着色器程序对象的标识符。

         * 当调用 glDeleteProgram 删除一个着色器程序时，如果该程序对象当前仍被使用（例如，通过 glUseProgram 激活），程序对象不会立即销毁。
         * 相反，它会在不再被使用时才实际销毁。这意味着你可以安全地调用 glDeleteProgram 删除一个仍在渲染管线中使用的程序对象，OpenGL 会确保在适当的时间点释放资源。
        */
        glDeleteProgram(shader_program);
        shader_program = 0;
    }
}

void Shader::InnerUse() const
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

void Shader::Use() const
{
    for (auto &texture_tuple : texture_tuples)
    {
        texture_tuple.texture->Use(texture_tuple.idx);
    }

    InnerUse();
}

bool Shader::IsValidProgram() const
{
    return shader_program > 0;
}

void Shader::SetTexture(const std::string &name, const Texture *texture)
{
    SetInt(name, texture_idx);

    texture_tuples.push_back({texture_idx, texture});

    texture_idx++;
}

// 向shader传递bool值
void Shader::SetBool(const std::string &name, const GLboolean value) const
{
    InnerUse();

    GLint location = GetUniformLocation(name);
    glUniform1i(location, (GLint)value);
}

// 向shader传递int值
void Shader::SetInt(const std::string &name, const GLint value) const
{
    InnerUse();

    GLint location = GetUniformLocation(name);
    glUniform1i(location, value);
}

// 向shader传递float值
void Shader::SetFloat(const std::string &name, const GLfloat value) const
{
    InnerUse();

    GLint location = GetUniformLocation(name);
    glUniform1f(location, value);
}

// 向shader传递浮点型vec4值
void Shader::SetFloat4(const std::string &name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const
{
    InnerUse();

    GLint location = GetUniformLocation(name);

    /*
     * glUniform4f函数用于设置uniform变量的值，特别是那些类型为四元素浮点向量（vec4）的uniform变量。
     * 需要先调用glUseProgram，因为它会在当前活动的着色器程序上设置统一变量。

     * 函数原型：GLint glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
     *  location：uniform变量的位置索引，这个位置是通过glGetUniformLocation函数获取的。
     *  v0到v3：分别是四元素向量的x、y、z和w分量的值。
    */
    glUniform4f(location, v0, v1, v2, v3);
}

void Shader::SetMat4f(const std::string &name, const glm::mat4 &matrix) const
{
    InnerUse();

    GLuint location = GetUniformLocation(name);

    /*
     * glUniformMatrix4fv 是 OpenGL 中用于设置着色器程序中的 uniform 矩阵变量的函数。
     * 具体来说，它将 4x4 浮点矩阵数组传递给当前激活的着色器程序中的指定 uniform 变量。

     * 函数原型：void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
     *  location：指定 uniform 变量的位置。这个位置值通常通过调用 glGetUniformLocation 函数获取。
     *  count：指定要传递的矩阵数量。对于单个 4x4 矩阵，这个值通常是 1。如果你需要传递多个矩阵，可以设置为相应的数量。
     *  transpose：指定是否要转置矩阵传递给着色器。GL_FALSE 表示矩阵是按列主序存储的（即不转置），GL_TRUE 表示矩阵是按行主序存储的（即需要转置）。大多数情况下，这个值设为 GL_FALSE，因为 OpenGL 使用列主序矩阵。
     *  value：指向包含矩阵数据的数组的指针。矩阵数据应按照列主序存储，即矩阵的第一个列的元素在数组的前四个位置。
    */
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetMat3f(const std::string &name, const glm::mat3 &matrix) const
{
    InnerUse();

    GLuint location = GetUniformLocation(name);
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetVec3f(const std::string &name, const glm::vec3 &vector) const
{
    InnerUse();

    GLuint location = GetUniformLocation(name);
    glUniform3f(location, vector.x, vector.y, vector.z);
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
