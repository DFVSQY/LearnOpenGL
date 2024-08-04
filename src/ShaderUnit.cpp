#include "ShaderUnit.h"
#include <fstream>
#include <iostream>
#include <sstream>

ShaderUnit::ShaderUnit(const std::string &path, const GLenum shaderType) : shader_id(0)
{
    const std::string shader_content = ReadShaderFile(path);
    if (shader_content.empty())
    {
        std::cerr << "Failed to read shader file: " << path << std::endl;
        return;
    }

    shader_id = Compile(shaderType, shader_content);
}

ShaderUnit::~ShaderUnit()
{
    if (shader_id > 0)
    {
        glDeleteShader(shader_id);
        shader_id = 0;
    }
}

GLuint ShaderUnit::GetShaderID() const
{
    return shader_id;
}

const std::string ShaderUnit::ReadShaderFile(const std::string &path) const
{
    const char *file_path = path.c_str();

    std::string str_content;
    std::ifstream shader_file;

    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        shader_file.open(file_path);
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
        std::cerr << "read shader error: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "read shader general_exception: " << e.what() << std::endl;
    }

    return str_content;
}

GLuint ShaderUnit::Compile(GLenum shaderType, const std::string &shaderCode)
{
    GLenum shader_type = shaderType;
    const char *shader_code = shaderCode.c_str();

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
    glShaderSource(shader, 1, &shader_code, NULL);

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