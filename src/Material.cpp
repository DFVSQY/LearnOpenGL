#include "Material.h"
#include "Texture.h"

Material::Material() : shader(nullptr), texture_tuples(), texture_idx(0)
{
}

Material::~Material()
{
}

void Material::Init(Shader *shader)
{
    this->shader = shader;
}

void Material::SetTexture(const std::string &name, Texture *texture)
{
    shader->Use();
    shader->SetInt(name, texture_idx);

    texture_tuples.push_back({texture_idx, texture, name});

    texture_idx++;
}

void Material::SetInt(const std::string &name, const GLint value)
{
    shader->Use();
    shader->SetInt(name, value);
}

void Material::SetMat4f(const std::string &name, const glm::mat4 &matrix)
{
    shader->Use();
    shader->SetMat4f(name, matrix);
}

void Material::SetMat3f(const std::string &name, const glm::mat3 &matrix)
{
    shader->Use();
    shader->SetMat3f(name, matrix);
}

void Material::SetVec3f(const std::string &name, const glm::vec3 &vector)
{
    shader->Use();
    shader->SetVec3f(name, vector);
}

void Material::SetFloat(const std::string &name, const GLfloat value)
{
    shader->Use();
    shader->SetFloat(name, value);
}

void Material::Use()
{
    for (auto &texture_tuple : texture_tuples)
    {
        texture_tuple.texture->Use(texture_tuple.idx);
    }

    shader->Use();

    for (auto &texture_tuple : texture_tuples)
    {
        shader->SetInt(texture_tuple.name, texture_tuple.idx);
    }
}