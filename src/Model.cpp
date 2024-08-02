#include "Model.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "assimp/Importer.hpp"
#include "assimp/mesh.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include <iostream>
#include <string>
#include <vector>

Model::Model(const char *path)
{
    LoadModel(path);
}

Model::~Model()
{
}

void Model::LoadModel(const std::string &path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
    {
        std::cerr << "Import model error:" << importer.GetErrorString() << std::endl;
        return;
    }

    m_directory = path.substr(0, path.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene)
{
    // 处理节点的每个网格
    for (unsigned int idx = 0; idx < node->mNumMeshes; idx++)
    {
        aiMesh *aiMesh = scene->mMeshes[node->mMeshes[idx]];
        m_meshes.push_back(ProcessMesh(aiMesh, scene));
    }

    // 处理子节点
    for (unsigned int idx = 0; idx < node->mNumChildren; idx++)
    {
        ProcessNode(node->mChildren[idx], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
    // todo: 临时代码，具体待定

    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    std::vector<VertexAttribute> attributes;
    Material *material = nullptr;

    return Mesh(vertices, indices, attributes, material);
}