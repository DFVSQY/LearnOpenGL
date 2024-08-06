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

    SetupModel_2();
}

////////////////////////////////////////////////// 配置渲染用的材质和网格 ///////////////////////////////////////////////

/*
 * 配置渲染用的材质1
*/
Shader *Scene::SetupMat_1()
{
    // Shader
    Shader *shader = LoadShader("../shaders/vertex_01.glsl", "../shaders/fragment_01.glsl");
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

    Mesh *mesh = new Mesh(vertices_vec, indices_vec, VertexAttributePresets::GetPosColorTexLayout(), shader);
    AddMesh(mesh);

    return mesh;
}

/*
 * 配置渲染用的材质2
*/
Shader *Scene::SetupMat_2()
{
    // Shader
    Shader *shader = LoadShader("../shaders/vertex_02.glsl", "../shaders/fragment_02.glsl");
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

    Mesh *mesh = new Mesh(vertices, indices, VertexAttributePresets::GetPosColorTexLayout(), shader);
    AddMesh(mesh);

    return mesh;
}

Shader *Scene::SetupMat_3()
{
    // Shader
    Shader *shader = LoadShader("../shaders/vertex_03.glsl", "../shaders/fragment_03.glsl");
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

    Mesh *mesh = new Mesh(vertices, indices, VertexAttributePresets::GetPosNormalTexLayout(), shader);
    AddMesh(mesh);

    return mesh;
}

Shader *Scene::SetupMat_4()
{
    // Shader
    Shader *shader = LoadShader("../shaders/vertex_04.glsl", "../shaders/fragment_04.glsl");
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

    Mesh *mesh = new Mesh(vertices, indices, VertexAttributePresets::GetPosNormalTexLayout(), shader);
    AddMesh(mesh);

    return mesh;
}

Shader *Scene::SetupMat_5()
{
    // Shader
    Shader *shader = LoadShader("../shaders/vertex_05.glsl", "../shaders/fragment_05.glsl");
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
    Shader *shader = LoadShader("../shaders/vertex_06.glsl", "../shaders/fragment_06.glsl");
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
    Shader *shader = LoadShader("../shaders/vertex_07.glsl", "../shaders/fragment_07.glsl");
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
    Shader *shader = LoadShader("../shaders/vertex_08.glsl", "../shaders/fragment_08.glsl");
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

    for (size_t idx = 0; idx < m_meshes.size(); ++idx)
    {
        Mesh *mesh = m_meshes[idx];

        Shader &shader = mesh->GetShader();
        UpdateModelMatrix(shader);
        UpdateViewMatrix(shader);
        UpdateProjectionMatrix(shader);

        mesh->Draw();
    }

    std::function<void(Mesh *)> each_mesh_func = [this](Mesh *mesh) {
        Shader &shader = mesh->GetShader();

        UpdateModelMatrix(shader);
        UpdateViewMatrix(shader);
        UpdateProjectionMatrix(shader);
    };

    for (size_t idx = 0; idx < m_models.size(); ++idx)
    {
        Model *model = m_models[idx];
        model->ForeachMesh(each_mesh_func);
        model->Draw();
    }
}

void Scene::UpdateModelMatrix(Shader &shader)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

    shader.SetMat4f("model", model);

    /*
     * 将法向量从模型空间变换到世界空间中需要用到的矩阵
    */
    glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model)));
    shader.SetMat3f("normalMatrix", normal_matrix);
}

void Scene::UpdateViewMatrix(Shader &shader)
{
    glm::mat4 view = m_camera.GetViewMatrix();
    shader.SetMat4f("view", view);

    glm::vec3 pos = m_camera.GetPos();
    shader.SetVec3f("camPos", pos);
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