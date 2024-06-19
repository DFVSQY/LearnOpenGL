#include "Scene.h"
#include "Mesh.h"
#include "Shader.h"

Scene::Scene() : m_meshes(), m_shaders()
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
}

void Scene::Init()
{
    Shader *shader = new Shader();
    bool shader_succ = shader->Init("../shaders/vertex_01.glsl", "../shaders/fragment_01.glsl");
    if (!shader_succ)
    {
        return;
    }
    AddShader(shader);

    /*
    GLfloat vertices[] = {
        // 顶点数据
        -0.5f, -0.5f, 0.0f, // left  bottom
        0.5f,  -0.5f, 0.0f, // right bottom
        0.0f,  0.5f,  0.0f  // center top
    };
    Mesh *mesh = new Mesh();
    bool mesh_succ = mesh->Init(vertices, 9);
    */

    GLfloat vertices[] = {
        // postion          // color
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, // top right
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
        -0.5f, 0.5f,  0.0f, 0.2f, 0.8f, 0.5f, // top left
    };
    GLuint indices[] = {
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };
    Mesh *mesh = new Mesh();
    bool mesh_succ = mesh->Init_Elements(vertices, indices, 24, 6);
    if (!mesh_succ)
    {
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

void Scene::Render()
{
    for (size_t i = 0; i < m_meshes.size(); ++i)
    {
        Shader *shader = m_shaders[i];
        shader->Use();

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