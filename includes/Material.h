#pragma once

#include <unordered_map>
#include "Shader.h"
#include "Texture.h"
#include <string>

class Material
{
  private:
    Shader *shader;

    std::unordered_map<int, Texture *> map_textures;

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