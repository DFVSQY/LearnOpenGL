#include "Material.h"
#include "Texture.h"

Material::Material() : shader(nullptr), map_textures(), texture_idx(0)
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

    map_textures.insert(std::make_pair(texture_idx, texture));

    texture_idx++;
}

void Material::SetInt(const std::string &name, int value)
{
    shader->Use();
    shader->SetInt(name, value);
}

void Material::SetMat4f(const std::string &name, const glm::mat4 &matrix)
{
    shader->Use();
    shader->SetMat4f(name, matrix);
}

void Material::Use()
{
    for (auto &texture : map_textures)
    {
        texture.second->Use(texture.first);
    }
    shader->Use();
}