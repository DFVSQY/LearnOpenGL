#include "Scene.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/ext/matrix_transform.hpp"

Scene::Scene() : m_meshes(), m_shaders(), m_textures()
{
}

Scene::~Scene()
{
    for (auto mesh : m_meshes)
    {
        delete mesh;
    }

    for (auto shader : m_shaders)
    {
        delete shader;
    }

    for (auto texture : m_textures)
    {
        delete texture;
    }

    for (auto material : m_materials)
    {
        delete material;
    }
}

void Scene::Init()
{
    // Shader
    Shader *shader = LoadShader("../shaders/vertex_01.glsl", "../shaders/fragment_01.glsl");
    if (!shader)
        return;

    // 纹理
    Texture *texture = LoadTexture("../textures/wall.jpg", GL_RGB);
    if (!texture)
        return;

    // 第二张纹理
    Texture *texture2 = LoadTexture("../textures/awesomeface.png", GL_RGBA);
    if (!texture2)
        return;

    Material *material = GenMaterial(shader);

    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, glm::vec3(0.5f));
    material->SetMat4f("trans", trans);

    material->SetTexture("texture0", texture);
    material->SetTexture("texture1", texture2);

    // 网格
    GLfloat vertices[] = {
        // postion          // color
        0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // top right
        0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // top left
    };
    GLuint indices[] = {
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };
    Mesh *mesh = new Mesh();
    bool mesh_succ = mesh->Init_Elements(vertices, indices, 32, 6, material);
    if (!mesh_succ)
    {
        delete mesh;
        return;
    }
    AddMesh(mesh);
}

Shader *Scene::LoadShader(const char *vertexFilePath, const char *fragmentFilePath)
{
    Shader *shader = new Shader();
    bool shader_succ = shader->Init(vertexFilePath, fragmentFilePath);
    if (!shader_succ)
    {
        delete shader;
        return nullptr;
    }
    AddShader(shader);
    return shader;
}

Texture *Scene::LoadTexture(const char *filePath, GLenum format)
{
    Texture *texture = new Texture();
    bool texture_succ = texture->Init(filePath, format);
    if (!texture_succ)
    {
        delete texture;
        return nullptr;
    }
    AddTexture(texture);
    return texture;
}

Material *Scene::GenMaterial(Shader *shader)
{
    Material *material = new Material();
    material->Init(shader);
    return material;
}

void Scene::AddMesh(Mesh *mesh)
{
    m_meshes.push_back(mesh);
}

void Scene::AddShader(Shader *shader)
{
    m_shaders.push_back(shader);
}

void Scene::AddTexture(Texture *texture)
{
    m_textures.push_back(texture);
}

void Scene::Render()
{
    for (size_t i = 0; i < m_meshes.size(); ++i)
    {
        Mesh *mesh = m_meshes[i];
        mesh->Draw();
    }
}