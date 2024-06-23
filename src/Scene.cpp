#include "Scene.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

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
}

void Scene::Init()
{
    // Shader
    Shader *shader = new Shader();
    bool shader_succ = shader->Init("../shaders/vertex_01.glsl", "../shaders/fragment_01.glsl");
    if (!shader_succ)
    {
        delete shader;
        return;
    }
    AddShader(shader);

    // 纹理
    Texture *texture = new Texture();
    bool texture_succ = texture->Init("../textures/wall.jpg", GL_RGB);
    if (!texture_succ)
    {
        delete texture;
        return;
    }
    AddTexture(texture);

    // 第二张纹理
    Texture *texture2 = new Texture();
    bool texture_succ2 = texture2->Init("../textures/awesomeface.png", GL_RGBA);
    if (!texture_succ2)
    {
        delete texture2;
        return;
    }
    AddTexture(texture2);

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
    bool mesh_succ = mesh->Init_Elements(vertices, indices, 32, 6);
    if (!mesh_succ)
    {
        delete mesh;
        return;
    }
    AddMesh(mesh);
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
        Texture *texture = m_textures[i];
        texture->Use(0);

        Texture *texture2 = m_textures[i + 1];
        texture2->Use(1);

        Shader *shader = m_shaders[i];
        shader->Use();
        shader->SetInt("texture0", 0);
        shader->SetInt("texture1", 1);

        /* 测试传递uniform值
        if (i == 0)
        {
            float timeValue = glfwGetTime();
            float color_value = (sin(timeValue) / 2.0f) + 0.5f;
            shader->SetFloat4("userColor", color_value / 2.0f, color_value, color_value / 3.0f, 1.0f);
        }
        */

        Mesh *mesh = m_meshes[i];
        mesh->Draw();
    }
}