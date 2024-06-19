#pragma once

#include "Mesh.h"
#include "Shader.h"
#include <vector>

class Scene
{
  private:
    std::vector<Mesh *> m_meshes;
    std::vector<Shader *> m_shaders;

  public:
    Scene();
    ~Scene();

    void Init();

    void AddMesh(Mesh *mesh);
    void AddShader(Shader *shader);

    void Render();
};