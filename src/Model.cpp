#include "Model.h"
#include "Mesh.h"
#include "Shader.h"
#include "ShaderUnit.h"
#include "VertexAttribute.h"
#include "assimp/Importer.hpp"
#include "assimp/mesh.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/vector3.h"
#include <iostream>
#include <string>
#include <vector>

Model::Model(const char *path)
{
    LoadModel(path);
}

Model::~Model()
{
    for (auto mesh : m_meshes)
        delete mesh;
    m_meshes.clear();

    for (auto shader : m_shaders)
        delete shader;
    m_shaders.clear();
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

    ShaderUnit vertex_unit = ShaderUnit("../shaders/vertex_08.glsl", GL_VERTEX_SHADER);
    ShaderUnit fragment_unit = ShaderUnit("../shaders/fragment_08.glsl", GL_FRAGMENT_SHADER);

    ProcessNode(scene->mRootNode, scene, vertex_unit, fragment_unit);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene, ShaderUnit &vertexUnit, ShaderUnit &fragmentUnit)
{
    // 处理节点的每个网格
    for (unsigned int idx = 0; idx < node->mNumMeshes; idx++)
    {
        aiMesh *aiMesh = scene->mMeshes[node->mMeshes[idx]];
        m_meshes.push_back(ProcessMesh(aiMesh, scene, vertexUnit, fragmentUnit));
    }

    // 处理子节点
    for (unsigned int idx = 0; idx < node->mNumChildren; idx++)
    {
        ProcessNode(node->mChildren[idx], scene, vertexUnit, fragmentUnit);
    }
}

Mesh *Model::ProcessMesh(aiMesh *mesh, const aiScene *scene, ShaderUnit &vertexUnit, ShaderUnit &fragmentUnit)
{
    std::vector<GLfloat> vertices;

    // 顶点列表
    for (unsigned int idx = 0; idx < mesh->mNumVertices; idx++)
    {
        // 位置
        aiVector3D position = mesh->mVertices[idx];
        vertices.push_back(position.x);
        vertices.push_back(position.y);
        vertices.push_back(position.z);

        // 法线
        aiVector3D normal = mesh->mNormals[idx];
        vertices.push_back(normal.x);
        vertices.push_back(normal.y);
        vertices.push_back(normal.z);

        // 纹理坐标
        if (mesh->mTextureCoords[0])
        {
            aiVector3D texCoord = mesh->mTextureCoords[0][idx];
            vertices.push_back(texCoord.x);
            vertices.push_back(texCoord.y);
        }
        else
        {
            vertices.push_back(0.0f);
        }
    }

    std::vector<GLuint> indices;

    // 索引列表
    for (unsigned int idx = 0; idx < mesh->mNumFaces; idx++)
    {
        aiFace face = mesh->mFaces[idx];
        for (unsigned int idx2 = 0; idx2 < face.mNumIndices; idx2++)
        {
            indices.push_back(face.mIndices[idx2]);
        }
    }

    // todo: 材质待定
    Shader shader(vertexUnit, fragmentUnit);

    Mesh *new_mesh = new Mesh(vertices, indices, VertexAttributePresets::GetPosNormalTexLayout(), shader);

    return new_mesh;
}
