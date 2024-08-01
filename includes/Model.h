#pragma once

#include "Mesh.h"
#include "assimp/scene.h"
#include <vector>

class Model
{
  private:
    std::vector<Mesh> m_meshes;

    std::string m_directory;

    void LoadModel(const std::string &path);

    void ProcessNode(aiNode *node, const aiScene *scene);
    void ProcessMesh(aiMesh *mesh, const aiScene *scene);

  public:
    // 删除复制构造函数和赋值操作符
    Model(const Model &) = delete;
    Model &operator=(const Model &) = delete;

    Model(const char *path);
    ~Model();
};