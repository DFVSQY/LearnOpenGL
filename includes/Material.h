#pragma once

#include "Shader.h"
#include "Texture.h"
#include <string>
#include <vector>

class Material
{
  private:
    struct TextureTuple
    {
        const int idx;
        const Texture *texture;
        const std::string name;
    };

    Shader *shader;

    std::vector<TextureTuple> texture_tuples;

    int texture_idx;

  public:
    Material();
    ~Material();

    void Init(Shader *shader);

    void SetTexture(const std::string &name, Texture *texture);
    void SetInt(const std::string &name, const GLint value);
    void SetMat4f(const std::string &name, const glm::mat4 &matrix);
    void SetMat3f(const std::string &name, const glm::mat3 &matrix);
    void SetVec3f(const std::string &name, const glm::vec3 &vector);
    void SetFloat(const std::string &name, const GLfloat value);

    void Use();
};