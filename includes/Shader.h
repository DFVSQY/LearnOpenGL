#pragma once

#include <string>
#include <vector>
#include "Texture2D.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "ShaderUnit.h"

class Shader
{
  private:
    struct TexturePair
    {
        const int idx;
        const Texture2D *texture;
    };

    GLuint shader_program;

    int texture_idx;

    std::vector<TexturePair> texture_tuples;

    std::string ReadShaderFile(const char *filePath);

    GLuint Link(GLuint vertexShader, GLuint fragmentShader);

    GLint GetUniformLocation(const std::string &name) const;

    void InnerUse() const;

  public:
    // 删除复制构造函数和赋值操作符
    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    Shader(const ShaderUnit &vertexShader, const ShaderUnit &fragmentShader);
    ~Shader();

    void SetTexture(const std::string &name, const Texture2D *texture);
    void SetBool(const std::string &name, const GLboolean value) const;
    void SetInt(const std::string &name, const GLint value) const;
    void SetFloat(const std::string &name, const GLfloat value) const;
    void SetFloat4(const std::string &name, const GLfloat v0, const GLfloat v1, const GLfloat v2,
                   const GLfloat v3) const;
    void SetMat4f(const std::string &name, const glm::mat4 &matrix) const;
    void SetMat3f(const std::string &name, const glm::mat3 &matrix) const;
    void SetVec3f(const std::string &name, const glm::vec3 &vector) const;

    void Use() const;

    bool IsValidProgram() const;
};