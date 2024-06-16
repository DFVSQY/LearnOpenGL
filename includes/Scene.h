#pragma once

#include "Mesh.h"
#include <vector>

class Scene
{
  private:
    std::vector<Mesh *> m_meshes;

  public:
    Scene();
    ~Scene();

    void Init();

    void AddMesh(Mesh *mesh);

    void Render();
};