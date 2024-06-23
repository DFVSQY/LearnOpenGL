#pragma once

#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include <vector>

class Scene
{
  private:
    std::vector<Mesh *> m_meshes;
    std::vector<Shader *> m_shaders;
    std::vector<Texture *> m_textures;
    std::vector<Material *> m_materials;

    void AddMesh(Mesh *mesh);
    void AddShader(Shader *shader);
    void AddTexture(Texture *texture);
    void AddMaterial(Material *material);

    Shader *LoadShader(const char *vertextPath, const char *fragmentPath);
    Texture *LoadTexture(const char *texturePath, GLenum format);
    Material *GenMaterial(Shader *shader);

  public:
    Scene();
    ~Scene();

    // 删除复制构造函数和赋值操作符
    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

    void Init();

    Material *SetupMat_1();
    Mesh *SetupMesh_1(Material *material);

    void Render();
};