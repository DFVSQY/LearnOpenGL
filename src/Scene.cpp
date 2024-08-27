#include "Scene.h"
#include "Mesh.h"
#include "Model.h"
#include "Shader.h"
#include "ShaderUnit.h"
#include "Texture.h"
#include "glm/ext/matrix_transform.hpp"
#include "GLFW/glfw3.h"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"
#include <vector>
#include "VertexAttribute.h"

Scene::Scene() : m_meshes(), m_shaders(), m_textures(), m_models(), m_camera()
{
    m_camSpeed = 0.05f;
    m_lastFrameTime = 0.0f;
    m_deltaTime = 0.0f;

    m_lastCursorPosX = 0;
    m_lastCursorPosY = 0;
    m_cursorSensitivity = 0.05;
    m_captureCursor = false;
}

Scene::~Scene()
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

    for (auto model : m_models)
        delete model;
    m_models.clear();
}

void Scene::Init(int width, int height)
{
    m_lastCursorPosX = (double)width / 2;
    m_lastCursorPosY = (double)height / 2;

    Shader *shader = SetupMat_8();
    SetupMesh_8(*shader);

    SetupMat_Outline();
}

////////////////////////////////////////////////// 配置渲染用的材质和网格 ///////////////////////////////////////////////

/*
 * 配置渲染用的材质1
*/
Shader *Scene::SetupMat_1()
{
    // Shader
    Shader *shader = LoadShader("../shaders/vertex_01.vert", "../shaders/fragment_01.frag");
    if (!shader)
        return nullptr;

    // 纹理
    Texture *texture = LoadTexture("../textures/wall.jpg", GL_RGB);
    if (!texture)
        return nullptr;

    // 第二张纹理
    Texture *texture2 = LoadTexture("../textures/awesomeface.png", GL_RGBA);
    if (!texture2)
        return nullptr;

    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, glm::vec3(0.5f));
    shader->SetMat4f("trans", trans);

    shader->SetTexture("texture0", texture);
    shader->SetTexture("texture1", texture2);

    AddShader(shader);

    return shader;
}

/*
 * 配置渲染用的网格1
*/
Mesh *Scene::SetupMesh_1(Shader &shader)
{
    const std::vector<GLfloat> vertices_vec = {
        // postion                      // color                      // 纹理坐标
        0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // top right
        0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // top left
    };

    const std::vector<GLuint> indices_vec = {
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };

    Mesh *mesh = new Mesh(vertices_vec, indices_vec, VertexAttributePresets::GetPosColorTexLayout(), &shader);
    AddMesh(mesh);

    return mesh;
}

/*
 * 配置渲染用的材质2
*/
Shader *Scene::SetupMat_2()
{
    // Shader
    Shader *shader = LoadShader("../shaders/vertex_02.vert", "../shaders/fragment_02.frag");
    if (!shader)
        return nullptr;

    // 纹理
    Texture *texture = LoadTexture("../textures/wall.jpg", GL_RGB);
    if (!texture)
        return nullptr;

    shader->SetTexture("texture0", texture);

    AddShader(shader);

    return shader;
}

/*
 * 配置渲染用的网格2
*/
Mesh *Scene::SetupMesh_2(Shader &shader)
{
    std::vector<GLfloat> vertices = {
        // postion                       // color                      // 纹理坐标
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, //
        0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, //
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //

        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //
        -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, //

        -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, //
        0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, //
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //

        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //
        -0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //
        -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, //

        -0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, //
        -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //

        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //
        -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, //
        -0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, //

        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, //
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //
        0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //

        0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //
        0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, //
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, //

        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //
        0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //
        0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, //

        0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, //
        -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, //
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //

        -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, //

        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, //
        -0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, //
        -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  //
    };

    std::vector<GLuint> indices = {
        0,  1,  2,  3,  4,  5,  // 前面
        6,  7,  8,  9,  10, 11, // 后面
        12, 13, 14, 15, 16, 17, // 左面
        18, 19, 20, 21, 22, 23, // 右面
        24, 25, 26, 27, 28, 29, // 底面
        30, 31, 32, 33, 34, 35  // 顶面
    };

    Mesh *mesh = new Mesh(vertices, indices, VertexAttributePresets::GetPosColorTexLayout(), &shader);
    AddMesh(mesh);

    return mesh;
}

Shader *Scene::SetupMat_3()
{
    // Shader
    Shader *shader = LoadShader("../shaders/vertex_03.vert", "../shaders/fragment_03.frag");
    if (!shader)
        return nullptr;

    // 纹理
    Texture *texture = LoadTexture("../textures/wall.jpg", GL_RGB);
    if (!texture)
        return nullptr;

    shader->SetTexture("texture0", texture);

    // 设置灯光的世界坐标位置
    // shader->SetVec3f("lightPos", glm::vec3(1.2f, 1.0f, 2.0f));

    // 设置灯光颜色
    // shader->SetVec3f("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    // 设置灯光属性
    shader->SetVec3f("light.position", glm::vec3(1.2f, 1.0f, 2.0f));
    shader->SetVec3f("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    shader->SetVec3f("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    shader->SetVec3f("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    // 设置物体材质属性
    shader->SetVec3f("material.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
    shader->SetVec3f("material.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    shader->SetVec3f("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    shader->SetFloat("material.shininess", 32.0f);

    AddShader(shader);

    return shader;
}

Mesh *Scene::SetupMesh_3(Shader &shader)
{
    std::vector<GLfloat> vertices = {
        // 位置                           // 法线                           // 纹理坐标
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f, //
        0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f, //
        0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, //
        0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, //
        -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 1.0f, //
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f, //

        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //
        0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, //
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //
        -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, //
        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //

        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 1.0f, //
        -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 0.0f, //
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 0.0f, //
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 0.0f, //
        -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 1.0f, //
        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 1.0f, //

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, //
        0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 0.0f, //
        0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 0.0f, //
        0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 0.0f, //
        0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, //
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, //

        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 0.0f, //
        0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f, 0.0f, //
        0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 1.0f, //
        0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 1.0f, //
        -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f, 1.0f, //
        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 0.0f, //

        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f, //
        0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f, //
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, //
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, //
        -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f, //
        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f  //
    };

    std::vector<GLuint> indices = {
        0,  1,  2,  3,  4,  5,  // 前面
        6,  7,  8,  9,  10, 11, // 后面
        12, 13, 14, 15, 16, 17, // 左面
        18, 19, 20, 21, 22, 23, // 右面
        24, 25, 26, 27, 28, 29, // 底面
        30, 31, 32, 33, 34, 35  // 顶面
    };

    Mesh *mesh = new Mesh(vertices, indices, VertexAttributePresets::GetPosNormalTexLayout(), &shader);
    AddMesh(mesh);

    return mesh;
}

Shader *Scene::SetupMat_4()
{
    // Shader
    Shader *shader = LoadShader("../shaders/vertex_04.vert", "../shaders/fragment_04.frag");
    if (!shader)
        return nullptr;

    // 漫反射纹理
    Texture *diffuse_tex = LoadTexture("../textures/container2.png", GL_RGBA);
    if (!diffuse_tex)
        return nullptr;

    // 高光纹理
    Texture *specular_tex = LoadTexture("../textures/container2_specular.png", GL_RGBA);
    if (!specular_tex)
        return nullptr;

    // Shader *material = GenMaterial(shader);

    // 材质属性
    shader->SetTexture("material.diffuse", diffuse_tex);
    shader->SetTexture("material.specular", specular_tex);
    shader->SetFloat("material.shininess", 64.0f);

    // 设置灯光属性
    shader->SetVec3f("light.position", glm::vec3(1.2f, 1.0f, 2.0f));
    shader->SetVec3f("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    shader->SetVec3f("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    shader->SetVec3f("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    AddShader(shader);

    return shader;
}

Mesh *Scene::SetupMesh_4(Shader &shader)
{
    std::vector<GLfloat> vertices = {
        // 位置                           // 法线                           // 纹理坐标
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f, //
        0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f, //
        0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, //
        0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, //
        -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 1.0f, //
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f, //

        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //
        0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, //
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //
        -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, //
        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //

        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 1.0f, //
        -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 0.0f, //
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 0.0f, //
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 0.0f, //
        -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 1.0f, //
        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 1.0f, //

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, //
        0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 0.0f, //
        0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 0.0f, //
        0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 0.0f, //
        0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, //
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, //

        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 0.0f, //
        0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f, 0.0f, //
        0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 1.0f, //
        0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 1.0f, //
        -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f, 1.0f, //
        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 0.0f, //

        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f, //
        0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f, //
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, //
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, //
        -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f, //
        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f  //
    };

    std::vector<GLuint> indices = {
        0,  1,  2,  3,  4,  5,  // 前面
        6,  7,  8,  9,  10, 11, // 后面
        12, 13, 14, 15, 16, 17, // 左面
        18, 19, 20, 21, 22, 23, // 右面
        24, 25, 26, 27, 28, 29, // 底面
        30, 31, 32, 33, 34, 35  // 顶面
    };

    Mesh *mesh = new Mesh(vertices, indices, VertexAttributePresets::GetPosNormalTexLayout(), &shader);
    AddMesh(mesh);

    return mesh;
}

Shader *Scene::SetupMat_5()
{
    // Shader
    Shader *shader = LoadShader("../shaders/vertex_05.vert", "../shaders/fragment_05.frag");
    if (!shader)
        return nullptr;

    // 漫反射纹理
    Texture *diffuse_tex = LoadTexture("../textures/container2.png", GL_RGBA);
    if (!diffuse_tex)
        return nullptr;

    // 高光纹理
    Texture *specular_tex = LoadTexture("../textures/container2_specular.png", GL_RGBA);
    if (!specular_tex)
        return nullptr;

    // 材质属性
    shader->SetTexture("material.diffuse", diffuse_tex);
    shader->SetTexture("material.specular", specular_tex);
    shader->SetFloat("material.shininess", 64.0f);

    // 设置灯光属性
    shader->SetVec3f("light.direction", glm::vec3(-0.0f, -0.0f, -5.0f));
    shader->SetVec3f("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    shader->SetVec3f("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    shader->SetVec3f("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    AddShader(shader);

    return shader;
}

Mesh *Scene::SetupMesh_5(Shader &shader)
{
    return SetupMesh_4(shader);
}

Shader *Scene::SetupMat_6()
{
    // Shader
    Shader *shader = LoadShader("../shaders/vertex_06.vert", "../shaders/fragment_06.frag");
    if (!shader)
        return nullptr;

    // 漫反射纹理
    Texture *diffuse_tex = LoadTexture("../textures/container2.png", GL_RGBA);
    if (!diffuse_tex)
        return nullptr;

    // 高光纹理
    Texture *specular_tex = LoadTexture("../textures/container2_specular.png", GL_RGBA);
    if (!specular_tex)
        return nullptr;

    // 材质属性
    shader->SetTexture("material.diffuse", diffuse_tex);
    shader->SetTexture("material.specular", specular_tex);
    shader->SetFloat("material.shininess", 64.0f);

    // 点光源属性
    shader->SetVec3f("light.position", glm::vec3(0.0f, 0.0f, 3.0f));
    shader->SetFloat("light.constant", 1.0f);
    shader->SetFloat("light.linear", 0.045f);
    shader->SetFloat("light.quadratic", 0.0075f);

    // 灯光的光照属性
    shader->SetVec3f("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    shader->SetVec3f("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    shader->SetVec3f("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    AddShader(shader);

    return shader;
}

Mesh *Scene::SetupMesh_6(Shader &shader)
{
    return SetupMesh_4(shader);
}

Shader *Scene::SetupMat_7()
{
    // Shader
    Shader *shader = LoadShader("../shaders/vertex_07.vert", "../shaders/fragment_07.frag");
    if (!shader)
        return nullptr;

    // 漫反射纹理
    Texture *diffuse_tex = LoadTexture("../textures/container2.png", GL_RGBA);
    if (!diffuse_tex)
        return nullptr;

    // 高光纹理
    Texture *specular_tex = LoadTexture("../textures/container2_specular.png", GL_RGBA);
    if (!specular_tex)
        return nullptr;

    // 材质属性
    shader->SetTexture("material.diffuse", diffuse_tex);
    shader->SetTexture("material.specular", specular_tex);
    shader->SetFloat("material.shininess", 64.0f);

    // 聚光灯属性
    shader->SetVec3f("light.position", m_camera.GetPos());
    shader->SetVec3f("light.direction", m_camera.GetFront());
    shader->SetFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
    shader->SetFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
    shader->SetFloat("light.constant", 1.0f);
    shader->SetFloat("light.linear", 0.045f);
    shader->SetFloat("light.quadratic", 0.0075f);

    // 灯光的光照属性
    shader->SetVec3f("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    shader->SetVec3f("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    shader->SetVec3f("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    AddShader(shader);

    return shader;
}

Mesh *Scene::SetupMesh_7(Shader &shader)
{
    return SetupMesh_4(shader);
}

Shader *Scene::SetupMat_8()
{
    // Shader
    Shader *shader = LoadShader("../shaders/vertex_08.vert", "../shaders/fragment_08.frag");
    if (!shader)
        return nullptr;

    // 漫反射纹理
    Texture *diffuse_tex = LoadTexture("../textures/container2.png", GL_RGBA);
    if (!diffuse_tex)
        return nullptr;

    // 高光纹理
    Texture *specular_tex = LoadTexture("../textures/container2_specular.png", GL_RGBA);
    if (!specular_tex)
        return nullptr;

    // 材质属性
    shader->SetTexture("material.diffuse", diffuse_tex);
    shader->SetTexture("material.specular", specular_tex);
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
    shader->SetVec3f("spotLight.position", m_camera.GetPos());
    shader->SetVec3f("spotLight.direction", m_camera.GetFront());
    shader->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    shader->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
    shader->SetFloat("spotLight.constant", 1.0f);
    shader->SetFloat("spotLight.linear", 0.045f);
    shader->SetFloat("spotLight.quadratic", 0.0075f);
    shader->SetVec3f("spotLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    shader->SetVec3f("spotLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    shader->SetVec3f("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    AddShader(shader);

    return shader;
}

Mesh *Scene::SetupMesh_8(Shader &shader)
{
    return SetupMesh_4(shader);
}

Shader *Scene::SetupMat_Outline()
{
    Shader *shader = LoadShader("../shaders/vertex_outline.vert", "../shaders/fragment_outline.frag");
    if (!shader)
        return nullptr;

    AddShader(shader);

    return shader;
}

void Scene::SetupModel_1()
{
    Model *model = new Model("../models/nanosuit/nanosuit.obj");
    if (!model->HasValidMesh())
    {
        delete model;
        return;
    }

    AddModel(model);
}

void Scene::SetupModel_2()
{
    Model *model = new Model("../models/Skull/12140_Skull_v3_L2.obj");
    if (!model->HasValidMesh())
    {
        delete model;
        return;
    }

    AddModel(model);
}

void Scene::InitMVP(Shader *shader)
{
    /* 
     * MVP矩阵初始化为单位矩阵
    */
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    /*
     * 对模型矩阵应用旋转变换，旋转角度随时间变化（glfwGetTime()返回程序运行时间），
     * 旋转轴为(0.5, 1.0, 0.0)，这是一个自定义的轴，使得旋转看起来更自然。
    */
    // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

    /*
     * 这块比较难理解，首先应该明白视图矩阵的作用是将模型从世界坐标系变换到观察者坐标系，
     * 通过在世界坐标系中移动相机得到的矩阵实际上是视图矩阵的逆矩阵，
     * 所以此处需要求取逆矩阵才能得到正确的视图矩阵。
    */
    // view = glm::translate(view, glm::vec3(0.0f, 0.0f, 3.0f));
    // view = glm::inverse(view);

    view = m_camera.GetViewMatrix();
    projection = m_camera.GetProjectionMatrix();

    shader->SetMat4f("model", model);
    shader->SetMat4f("view", view);
    shader->SetMat4f("projection", projection);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Shader *Scene::LoadShader(const std::string &vertexFilePath, const std::string &fragmentFilePath)
{
    ShaderUnit vertex_unit = ShaderUnit(vertexFilePath, GL_VERTEX_SHADER);
    ShaderUnit fragment_unit = ShaderUnit(fragmentFilePath, GL_FRAGMENT_SHADER);

    Shader *shader = new Shader(vertex_unit, fragment_unit);

    if (!shader->IsValidProgram())
    {
        delete shader;
        return nullptr;
    }

    return shader;
}

Texture *Scene::LoadTexture(const std::string &filePath, GLenum format)
{
    Texture *texture = new Texture();
    bool texture_succ = texture->Init(filePath.c_str(), format);
    if (!texture_succ)
    {
        delete texture;
        return nullptr;
    }
    AddTexture(texture);
    return texture;
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

void Scene::AddModel(Model *model)
{
    m_models.push_back(model);
}

void Scene::Render()
{
    float now_time = glfwGetTime();
    if (m_lastFrameTime <= 0.0f)
        m_lastFrameTime = now_time;

    m_deltaTime = now_time - m_lastFrameTime;
    m_lastFrameTime = now_time;

    // 单网格渲染
    if (!m_meshes.empty())
    {
        Mesh *mesh = m_meshes[0];
        Shader *shader = m_shaders[0];
        Shader *outlineShader = m_shaders[1];
        DrawMeshAndOutline(mesh, shader, outlineShader);
    }

    // 单模型渲染
    if (!m_models.empty())
    {
        std::function<void(Mesh *)> each_mesh_func = [this](Mesh *mesh) {
            Shader &shader = mesh->GetShader();

            UpdateModelMatrix(shader);
            UpdateViewMatrix(shader);
            UpdateProjectionMatrix(shader);
        };

        Model *model = m_models[0];
        model->ForeachMesh(each_mesh_func);
        model->Draw();
    }
}

/*
 * 绘制单网格及其轮廓
 * 基本原理：
 *  1. 第一个遍正常渲染物体同时写入模板值
 *  2. 第二遍渲染轮廓 （第一轮写入了模板值的需要模板测试失败从而禁止写入颜色缓冲）
*/
void Scene::DrawMeshAndOutline(Mesh *mesh, Shader *shader, Shader *outlineShader)
{
    // 第一个遍正常渲染物体，
    // 同时片元展示区域的模板缓冲写入模板值1
    {
        /*
         * 在深入解释这个函数之前，先要理解模板缓冲区（Stencil Buffer）和模板测试（Stencil Test）的基本概念。

         * 模板缓冲区（Stencil Buffer）：
         *  模板缓冲区是一个与颜色缓冲区和深度缓冲区类似的附加缓冲区。它通常被用来在渲染过程中对某些像素执行复杂的遮罩或剪裁操作。
         *  每个像素在模板缓冲区中都有一个对应的值，这些值可以用来决定像素是否应该被绘制。

         * 模板测试（Stencil Test）：
         *  在渲染过程中，模板测试会针对每个像素进行检查，以决定该像素是否会通过并被渲染。模板测试通常包含两个步骤：

         * 比较测试：
         *  根据设定的模板函数（通过 glStencilFunc 配置）将当前模板缓冲区中的值与参考值进行比较，决定测试是否通过。

         * 模板操作：
         *  根据比较测试的结果和深度测试的结果，决定如何修改模板缓冲区中的值。这里就是 glStencilOp 函数发挥作用的地方。

         * 函数原型：void glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
         * 参数解释
         *  sfail: 当模板测试失败时，指定对模板缓冲区的操作。
         *  dpfail: 当模板测试通过但深度测试失败时，指定对模板缓冲区的操作。
         *  dppass: 当模板测试和深度测试都通过时，指定对模板缓冲区的操作。

         * 可用的操作符
         * OpenGL 提供了几种可用的操作符，用于控制模板缓冲区中的值：
         *  GL_KEEP: 保持当前模板缓冲区的值不变。
         *  GL_ZERO: 将模板缓冲区的值设置为0。
         *  GL_REPLACE: 将模板缓冲区的值设置为参考值（由 glStencilFunc 设置）。
         *  GL_INCR: 增加当前模板缓冲区的值。如果值已经是最大值，则保持不变。
         *  GL_INCR_WRAP: 增加当前模板缓冲区的值。如果值已经是最大值，则包裹为0。
         *  GL_DECR: 减少当前模板缓冲区的值。如果值已经是最小值，则保持不变。
         *  GL_DECR_WRAP: 减少当前模板缓冲区的值。如果值已经是最小值，则包裹为最大值。
         *  GL_INVERT: 按位反转当前模板缓冲区的值。
        */
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        /*
         * glStencilFunc 是 OpenGL 中用于设置模板测试（Stencil Test）行为的函数。
         * 它决定了模板缓冲区中存储的模板值如何与参考值进行比较，从而确定当前片段是否通过模板测试。

         * 函数原型：void glStencilFunc(GLenum func, GLint ref, GLuint mask);
         * 参数解释:
         *  func: 用于指定模板测试的比较函数。这是一个 GLenum 值，定义了模板缓冲区的值与参考值之间的比较方式。
         *  ref: 参考值（Reference Value），用于与模板缓冲区中的值进行比较。该值在比较时会先与 mask 进行按位与（bitwise AND）操作。
         *  mask: 一个掩码（Mask），用于屏蔽模板缓冲区的某些位（bit）。在进行比较时，模板缓冲区的值和参考值都会与这个掩码进行按位与操作。

         * func 可以是以下几个值之一：
         *  GL_NEVER: 从不通过测试。即使模板值和参考值完全相同，测试也不会通过。
         *  GL_ALWAYS: 始终通过测试。无论模板值与参考值是否匹配，测试都会通过。
         *  GL_LESS: 当模板值小于参考值时，通过测试。
         *  GL_LEQUAL: 当模板值小于或等于参考值时，通过测试。
         *  GL_GREATER: 当模板值大于参考值时，通过测试。
         *  GL_GEQUAL: 当模板值大于或等于参考值时，通过测试。
         *  GL_EQUAL: 当模板值等于参考值时，通过测试。
         *  GL_NOTEQUAL: 当模板值不等于参考值时，通过测试。
        */
        glStencilFunc(GL_ALWAYS, 1, 0xFF);

        /*
         * glStencilMask 是 OpenGL 中用于控制模板缓冲区的写入权限的函数。
         * 它定义了在渲染过程中哪些位可以被写入到模板缓冲区中。这允许你在渲染过程中对模板缓冲区进行细粒度的控制。

         * 函数原型：void glStencilMask(GLuint mask);
         * 参数解释:
         *  mask: 一个掩码值，用于指定哪些位可以被写入到模板缓冲区中。mask 是一个 32 位的无符号整数，其中的每一位对应于模板缓冲区中的一个位。

         * 当渲染一个片段并且通过了模板测试后，OpenGL 会将新的模板值写入到模板缓冲区中。这个新的值是片段的模板值和掩码应用后的结果。

         * glStencilOp 用于定义在模板测试后应如何处理模板缓冲区中的值，而 glStencilMask 则控制哪些位可以被写入。
         * 这两者通常需要一起使用，以实现所需的渲染效果。
        */
        glStencilMask(0xFF);

        UpdateModelMatrix(*shader);
        UpdateViewMatrix(*shader);
        UpdateProjectionMatrix(*shader);
        mesh->ChangeShader(shader);
        mesh->Draw();
    }

    // 第二遍渲染轮廓
    {
        /*
         * 模板缓冲区中的值不等于1时模板测试通过（即物体片元的渲染区域不会被渲染到）
        */
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

        /*
         * 禁止写入模板缓冲区
        */
        glStencilMask(0x00);

        /*
         * 在绘制物体轮廓时，禁止深度测试（Depth Test）是常见的做法。这主要是为了确保轮廓能够正确地渲染到物体的边缘上，而不被其他物体遮挡。
         * 轮廓通常是较薄的几何体（如线条），如果深度测试开启，这些线条可能会因为深度冲突（z-fighting）而变得不清晰或不连续。
         * 禁用深度测试可以避免这种情况，使轮廓的渲染效果更加清晰和稳定。
        */
        glDisable(GL_DEPTH_TEST);

        UpdateModelMatrix(*outlineShader, true);
        UpdateViewMatrix(*outlineShader, true);
        UpdateProjectionMatrix(*outlineShader);
        mesh->ChangeShader(outlineShader);
        mesh->Draw();
    }

    // 恢复深度测试
    {
        glStencilMask(0xFF); // 开启模板缓冲区写入，不开启则使用 glClear(GL_STENCIL_BUFFER_BIT) 清空无法写入清空值。
        glEnable(GL_DEPTH_TEST);
    }
}

void Scene::UpdateModelMatrix(Shader &shader, bool ignoreNotModel)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

    shader.SetMat4f("model", model);

    if (!ignoreNotModel) // 不忽略非 model 值时
    {
        /*
        * 将法向量从模型空间变换到世界空间中需要用到的矩阵
        */
        glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model)));
        shader.SetMat3f("normalMatrix", normal_matrix);
    }
}

void Scene::UpdateViewMatrix(Shader &shader, bool ignoreNotView)
{
    glm::mat4 view = m_camera.GetViewMatrix();
    shader.SetMat4f("view", view);

    if (!ignoreNotView) // 不忽略非 view 值时
    {
        glm::vec3 pos = m_camera.GetPos();
        shader.SetVec3f("camPos", pos);
    }
}

void Scene::UpdateProjectionMatrix(Shader &shader)
{
    glm::mat4 projection = m_camera.GetProjectionMatrix();
    shader.SetMat4f("projection", projection);
}

void Scene::MoveCamForward()
{
    m_camera.MoveForwardOrBackward(m_camSpeed);
}

void Scene::MoveCamBackward()
{
    m_camera.MoveForwardOrBackward(-m_camSpeed);
}

void Scene::MoveCamLeft()
{
    m_camera.MoveLeftOrRight(-m_camSpeed);
}

void Scene::MoveCamRight()
{
    m_camera.MoveLeftOrRight(m_camSpeed);
}

void Scene::UpdateCamYawAndPitch(double xPos, double yPos)
{
    if (!m_captureCursor)
    {
        m_captureCursor = true;
        m_lastCursorPosX = xPos;
        m_lastCursorPosY = yPos;
    }

    // 窗口中捕获的鼠标位置是从右上角到左下角坐标逐渐增大的
    double xOffset = xPos - m_lastCursorPosX;
    double yOffset = m_lastCursorPosY - yPos;

    m_lastCursorPosX = xPos;
    m_lastCursorPosY = yPos;

    m_camera.TuneYawAndPitch(xOffset * m_cursorSensitivity, yOffset * m_cursorSensitivity);
}

void Scene::UpdateCamZoom(double yOffset)
{
    m_camera.TuneZoom(yOffset);
}