#include "Model.h"
#include "Mesh.h"
#include "Shader.h"
#include "ShaderUnit.h"
#include "Texture2D.h"
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

    for (auto texture : m_textures)
        delete texture;
    m_textures.clear();
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
        ProcessMesh(aiMesh, scene, vertexUnit, fragmentUnit);
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

    Shader *shader = new Shader(vertexUnit, fragmentUnit);
    m_shaders.push_back(shader);

    // 材质
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture2D *> diffuse_textures = LoadMaterialTextures(material, aiTextureType_DIFFUSE);
        std::vector<Texture2D *> specular_textures = LoadMaterialTextures(material, aiTextureType_SPECULAR);

        if (!diffuse_textures.empty())
            shader->SetTexture("material.diffuse", diffuse_textures[0]);
        else
            shader->SetTexture("material.diffuse", Texture2D::GetWhite2DTexture());

        if (!specular_textures.empty())
            shader->SetTexture("material.specular", specular_textures[0]);
        else
            shader->SetTexture("material.specular", Texture2D::GetWhite2DTexture());
    }
    else
    {
        shader->SetTexture("material.diffuse", Texture2D::GetWhite2DTexture());
        shader->SetTexture("material.specular", Texture2D::GetWhite2DTexture());
    }
    shader->SetFloat("material.shininess", 64.0f);

    // 方向光属性
    shader->SetVec3f("dirLight.direction", glm::vec3(-0.0f, -0.0f, -5.0f));
    shader->SetVec3f("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    shader->SetVec3f("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    shader->SetVec3f("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    // 点光源属性
    shader->SetVec3f("pointLight.position", glm::vec3(0.0f, 0.0f, 3.0f));
    shader->SetFloat("pointLight.constant", 1.0f);
    shader->SetFloat("pointLight.linear", 0.045f);
    shader->SetFloat("pointLight.quadratic", 0.0075f);
    shader->SetVec3f("pointLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    shader->SetVec3f("pointLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    shader->SetVec3f("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    // 聚光灯属性
    shader->SetVec3f("spotLight.position", glm::vec3(0.0f, 0.0f, 5.0f));
    shader->SetVec3f("spotLight.direction", glm::vec3(0.0f, 0.0f, -1.0f));
    shader->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    shader->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
    shader->SetFloat("spotLight.constant", 1.0f);
    shader->SetFloat("spotLight.linear", 0.045f);
    shader->SetFloat("spotLight.quadratic", 0.0075f);
    shader->SetVec3f("spotLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    shader->SetVec3f("spotLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    shader->SetVec3f("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    Mesh *new_mesh = new Mesh(vertices, indices, VertexAttributePresets::GetPosNormalTexLayout(), shader);
    m_meshes.push_back(new_mesh);

    return new_mesh;
}

std::vector<Texture2D *> Model::LoadMaterialTextures(const aiMaterial *mat, const aiTextureType type)
{
    std::vector<Texture2D *> textures;

    unsigned int count = mat->GetTextureCount(type);
    for (unsigned int idx = 0; idx < count; idx++)
    {
        aiString str;
        mat->GetTexture(type, idx, &str);

        Texture2D *texture = new Texture2D();

        const std::string &file_path = m_directory + '/' + str.C_Str();
        texture->Init(file_path.c_str());

        m_textures.push_back(texture);
        textures.push_back(texture);
    }

    return textures;
}

void Model::ForeachMesh(std::function<void(Mesh *)> func) const
{
    for (auto mesh : m_meshes)
    {
        func(mesh);
    }
}

void Model::Draw() const
{
    for (auto mesh : m_meshes)
    {
        mesh->Draw();
    }
}

bool Model::HasValidMesh() const
{
    return !m_meshes.empty();
}