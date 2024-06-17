#include "Scene.h"
#include "Mesh.h"
#include "Shader.h"

Scene::Scene() : m_meshes()
{
}

Scene::~Scene()
{
    for (auto mesh : m_meshes)
    {
        delete mesh;
    }
}

void Scene::Init()
{
    Shader shader;
    GLuint shader_program = shader.Init("../shaders/vertex_01.glsl", "../shaders/fragment_01.glsl");
    if (shader_program == 0)
    {
        return;
    }

    /*
    GLfloat vertices[] = {
        // 顶点数据
        -0.5f, -0.5f, 0.0f, // left  bottom
        0.5f,  -0.5f, 0.0f, // right bottom
        0.0f,  0.5f,  0.0f  // center top
    };
    Mesh *mesh = new Mesh();
    bool mesh_succ = mesh->Init(vertices, 9, shader_program);
    */

    GLfloat vertices[] = {
        0.5f,  0.5f,  0.0f, // top right
        0.5f,  -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f  // top left
    };
    GLuint indices[] = {
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };
    Mesh *mesh = new Mesh();
    bool mesh_succ = mesh->Init_Elements(vertices, indices, 12, 6, shader_program);

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

void Scene::Render()
{
    for (auto mesh : m_meshes)
    {
        mesh->Draw();
    }
}