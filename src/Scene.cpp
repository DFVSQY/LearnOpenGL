#include "Scene.h"
#include "Mesh.h"
#include "Model.h"
#include "Shader.h"
#include "ShaderUnit.h"
#include "Texture.h"
#include "glm/ext/matrix_transform.hpp"
#include "GLFW/glfw3.h"
#include "glm/fwd.hpp"
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

    Shader *cube_shader = SetupMat_Enlarge();
    SetupCubeMesh(*cube_shader);
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

Shader *Scene::SetupMat_GlassWind()
{
    // Shader
    Shader *shader = LoadShader("../shaders/vertex_glasswind.vert", "../shaders/fragment_glasswind.frag");
    if (!shader)
        return nullptr;

    // 纹理
    Texture *texture = LoadTexture("../textures/glass_wnd.png", GL_RGBA);
    if (!texture)
        return nullptr;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.7f));
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
    shader->SetMat4f("model", model);

    shader->SetTexture("texture0", texture);

    AddShader(shader);

    return shader;
}

Shader *Scene::SetupMat_Grass()
{
    // Shader
    Shader *shader = LoadShader("../shaders/vertex_grass.vert", "../shaders/fragment_grass.frag");
    if (!shader)
        return nullptr;

    // 纹理
    Texture *texture = LoadTexture("../textures/grass.png", GL_RGBA, GL_CLAMP_TO_EDGE);
    if (!texture)
        return nullptr;

    glm::mat4 model = glm::mat4(1.0f);
    // model = glm::scale(model, glm::vec3(0.7f));
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
    shader->SetMat4f("model", model);

    shader->SetTexture("texture0", texture);

    AddShader(shader);

    return shader;
}

Shader *Scene::SetupMat_Enlarge()
{
    // Shader
    Shader *shader = LoadShader("../shaders/vertex_enlarge.vert", "../shaders/fragment_enlarge.frag");
    if (!shader)
        return nullptr;

    // 纹理
    Texture *texture = LoadTexture("../textures/wall.jpg", GL_RGB);
    if (!texture)
        return nullptr;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, 45.0f, glm::vec3(0.5f, 1.0f, 0.0f));
    shader->SetMat4f("model", model);

    shader->SetTexture("texture0", texture);

    AddShader(shader);

    return shader;
}

Mesh *Scene::SetupCubeMesh(Shader &shader)
{
    std::vector<GLfloat> vertices = {
        // Positions          // Normals           // Texture Coords
        // 前面
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  //
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   //
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  //

        // 后面
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, //
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  //
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,   //
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  //

        // 左面
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   //
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  //
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, //
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  //

        // 右面
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   //
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  //
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, //
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  //

        // 上面
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  //
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   //
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  //

        // 下面
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, //
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  //
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   //
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f   //
    };

    std::vector<GLuint> indices = {
        0,  1,  2,  2,  3,  0,  // 前面
        4,  5,  6,  6,  7,  4,  // 后面
        8,  9,  10, 10, 11, 8,  // 左面
        12, 13, 14, 14, 15, 12, // 右面
        16, 17, 18, 18, 19, 16, // 上面
        20, 21, 22, 22, 23, 20  // 下面
    };

    Mesh *mesh = new Mesh(vertices, indices, VertexAttributePresets::GetPosNormalTexLayout(), &shader);
    AddMesh(mesh);

    return mesh;
}

Mesh *Scene::SetupRectangleMesh(Shader &shader)
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

Texture *Scene::LoadTexture(const std::string &filePath, GLenum format, GLint wrapMode)
{
    Texture *texture = new Texture();
    bool texture_succ = texture->Init(filePath.c_str(), format, wrapMode);
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

    // 网格渲染
    if (!m_meshes.empty())
    {
        Mesh *mesh = m_meshes[0];
        DrawCullFace(mesh);
    }

    // 模型渲染
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

/*
 * 不开启混合的情况下，测试渲染半透玻璃的效果
*/
void Scene::DrawGlassWithoutBlend(Mesh *cube, Mesh *rectangle)
{
    // 先渲染后面的立方体
    {
        Mesh *cube_mesh = cube;
        Shader &cube_shader = cube_mesh->GetShader();
        UpdateModelMatrix(cube_shader);
        UpdateViewMatrix(cube_shader);
        UpdateProjectionMatrix(cube_shader);
        cube_mesh->Draw();
    }

    // 后渲染前面的半透玻璃
    {
        Mesh *rectangle_mesh = rectangle;
        Shader &rectangle_shader = rectangle_mesh->GetShader();
        UpdateViewMatrix(rectangle_shader, true);
        UpdateProjectionMatrix(rectangle_shader);
        rectangle_mesh->Draw();
    }
}

/*
 * 不开启混合的情况下，测试渲染半透玻璃的效果
*/
void Scene::DrawGlassWithBlend(Mesh *cube, Mesh *rectangle)
{
    // 先渲染后面的立方体
    {
        glDisable(GL_BLEND);
        Mesh *cube_mesh = cube;
        Shader &cube_shader = cube_mesh->GetShader();
        UpdateModelMatrix(cube_shader);
        UpdateViewMatrix(cube_shader);
        UpdateProjectionMatrix(cube_shader);
        cube_mesh->Draw();
    }

    // 后渲染前面的半透玻璃
    {
        /*
         * 'glEnable(GL_BLEND)'用于启用混合（Blending）操作。
         * 在渲染过程中，混合操作是将当前片段的颜色（即正在渲染的像素）与已经存在于帧缓冲区中的颜色进行组合，以生成最终输出的颜色。
         * 这在处理透明对象、半透明效果和一些特殊的光照效果时非常重要。
         * 只有在混合被启用后，OpenGL 才会对片段执行混合操作，否则它会直接用当前片段的颜色覆盖帧缓冲区中的颜色。

         * 混合操作通过以下步骤将片段的颜色与帧缓冲区中的颜色混合：        
         *  源颜色（Source Color）：即当前渲染片段的颜色。
         *  目标颜色（Destination Color）：即当前帧缓冲区中的颜色。
         *  混合操作的基本公式如下： 
         *      finalColor = (srcColor × srcFactor) + (dstColor × dstFactor)
         *      srcColor 是源颜色。
         *      dstColor 是目标颜色。
         *      srcFactor 和 dstFactor 是混合因子，控制源颜色和目标颜色的权重。

         * 使用 glBlendFunc 和 glBlendEquation 函数可以设置混合因子和混合方程，从而控制混合操作的行为。

         * 如果开启了混合（通过调用glEnable(GL_BLEND)），但没有显式调用glBlendFunc来指定混合因子，OpenGL会使用默认的混合因子进行混合操作。
         * 默认情况下，OpenGL 使用以下混合因子：
         *  glBlendFunc(GL_ONE, GL_ZERO)
         *      GL_ONE: 源因子被设置为 GL_ONE，表示源颜色将被完整保留（乘以 1）
         *      GL_ZERO: 目标因子被设置为 GL_ZERO，表示目标颜色不会影响最终颜色（乘以 0）

         * 最常见的 Alpha 混合模式
         *  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
         *  用于处理透明度，其中源颜色按其 Alpha 值进行缩放，而目标颜色按 (1 - Alpha) 进行缩放。

         * glBlendEquation(GLenum mode):
         *  mode 控制混合操作的方程方式，决定了如何将源颜色与目标颜色组合。常见的模式包括：
         *      GL_FUNC_ADD：将源颜色和目标颜色相加。
         *      GL_FUNC_SUBTRACT：用源颜色减去目标颜色。
         *      GL_FUNC_REVERSE_SUBTRACT：用目标颜色减去源颜色。
         * 默认情况下，OpenGL 使用加法混合。

         * 注意事项
         *  顺序问题：在渲染半透明物体时，顺序非常重要。通常需要按照从远到近的顺序进行渲染，以确保混合结果正确。
         *  性能影响：启用混合后，会增加 GPU 的计算负担，特别是在复杂场景中。这是因为每个片段都需要与帧缓冲区中的像素进行计算。
        */
        glEnable(GL_BLEND);

        /*
         * glBlendFunc 是 OpenGL 中的一个函数，用于指定在混合（Blending）操作中使用的混合因子。
         * 混合操作是指将当前片段的颜色与已经存在于帧缓冲区中的颜色进行组合，以生成最终的输出颜色。
         * 通过使用 glBlendFunc，你可以控制源颜色（即当前片段的颜色）和目标颜色（即帧缓冲区中的颜色）如何组合，从而实现各种视觉效果，例如透明度、加法混合等。

         * 函数原型：void glBlendFunc(GLenum sfactor, GLenum dfactor);
         * 参数：
         *  sfactor: 指定源颜色的混合因子。
         *  dfactor: 指定目标颜色的混合因子。

         * 混合因子（Blend Factor）在 OpenGL 中不是一个标量，而是一个向量，具体来说，是一个包含颜色分量的向量。
         * 混合因子影响混合操作中的每个颜色分量（红色、绿色、蓝色和 alpha）如何参与混合计算。

         * glBlendFunc 支持多种混合因子，以下是一些常见的混合因子及其含义：
         *  GL_ZERO：因子为 (0, 0, 0, 0)，即完全忽略该颜色（乘以 0）。
         *  GL_ONE：因子为 (1, 1, 1, 1)，即完全保留该颜色（乘以 1）。
         *  GL_SRC_COLOR：因子为源颜色的每个分量，即 (R_s, G_s, B_s, A_s)。
         *  GL_ONE_MINUS_SRC_COLOR：因子为1减去源颜色的每个分量，即 (1-R_s, 1-G_s, 1-B_s, 1-A_s)。
         *  GL_DST_COLOR：因子为目标颜色的每个分量，即 (R_d, G_d, B_d, A_d)。
         *  GL_ONE_MINUS_DST_COLOR：因子为1减去目标颜色的每个分量，即 (1-R_d, 1-G_d, 1-B_d, 1-A_d)。
         *  GL_SRC_ALPHA：因子为源颜色的Alpha分量，即 (A_s, A_s, A_s, A_s)。
         *  GL_ONE_MINUS_SRC_ALPHA：因子为1减去源颜色的Alpha分量，即 (1-A_s, 1-A_s, 1-A_s, 1-A_s)。
         *  GL_DST_ALPHA：因子为目标颜色的Alpha分量，即 (A_d, A_d, A_d, A_d)。
         *  GL_ONE_MINUS_DST_ALPHA：因子为1减去目标颜色的Alpha分量，即 (1-A_d, 1-A_d, 1-A_d, 1-A_d)。
         *  GL_CONSTANT_COLOR 和 GL_CONSTANT_ALPHA：因子为一个常量颜色或常量 alpha，即 (R_c, G_c, B_c, A_c)，其中 R_c, G_c, B_c, A_c 是通过 glBlendColor 设置的常量颜色或 alpha。
        */
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Mesh *rectangle_mesh = rectangle;
        Shader &rectangle_shader = rectangle_mesh->GetShader();
        UpdateViewMatrix(rectangle_shader, true);
        UpdateProjectionMatrix(rectangle_shader);
        rectangle_mesh->Draw();
    }
}

/*
 * 不开启混合的情况下，渲染有透明区域的草
*/
void Scene::DrawGrass(Mesh *cube, Mesh *rectangle)
{
    // 渲染后面的立方体
    {
        Mesh *cube_mesh = cube;
        Shader &cube_shader = cube_mesh->GetShader();
        UpdateModelMatrix(cube_shader);
        UpdateViewMatrix(cube_shader);
        UpdateProjectionMatrix(cube_shader);
        cube_mesh->Draw();
    }

    // 渲染前面的2D草
    /*
     * 在不开启Blend的情况下，渲染带有透明区域的纹理时，可以采用Alpha测试从而discard掉一些透明片元。
     * 比如草并不需要半透明，只需要根据纹理颜色值，显示一部分（完全不透明），或者不显示一部分（完全透明），没有中间情况。
     * 所以当添加像草这样的植被到场景中时，我们不希望看到草的方形图像，而是只显示草的部分，并能看透图像其余的部分。
     * 我们可以丢弃(Discard)显示纹理中透明部分的片段，不将这些片段存储到颜色缓冲中。

     * 在游戏开发中，处理带有透明区域的纹理时，确实通常会将纹理的 WrapMode 设置为 Clamp。
     * 这是为了避免在纹理采样时由于纹理坐标超出范围而导致的边缘问题，如黑边或其他不期望的瑕疵。

     * 在游戏开发中，草地的渲染处理通常是放在 非透明队列 中，而不是透明队列。
     * 为什么草地渲染通常在非透明队列
     *  深度写入与排序问题：将草地渲染放在透明队列中会导致复杂的深度排序问题，尤其是草地通常由大量的平面片段组成，这些片段之间的透明排序会非常复杂且影响性能。
     *                  而且，草地场景中往往存在大量的草叶片段，使用透明队列进行逐片段排序和混合的开销较大，容易导致性能下降。

     *  更高效的渲染：使用非透明队列渲染草地时，可以启用深度写入，从而确保正确的遮挡关系，这对于草地的大规模渲染特别重要。
     *              同时，由于草地片段相对较小且数量众多，从前到后的渲染顺序有助于提高渲染效率，因为后面片段可能被前面的草叶完全遮挡，减少了不必要的片段着色。

     * 使用Alpha测试或Alpha裁剪：为了处理草地的透明区域，开发者通常会使用Alpha测试或Alpha裁剪（Alpha Cutoff），这意味着只有那些Alpha值超过某个阈值的片段才会被渲染为不透明。
     *                      这种方法允许草叶片段在非透明队列中进行渲染，同时保留草地的形状和视觉效果。
    */
    {
        Mesh *rectangle_mesh = rectangle;
        Shader &rectangle_shader = rectangle_mesh->GetShader();
        UpdateViewMatrix(rectangle_shader, true);
        UpdateProjectionMatrix(rectangle_shader);
        rectangle_mesh->Draw();
    }
}

/*
 * 面剔除示例
*/
void Scene::DrawCullFace(Mesh *mesh)
{
    /*
     * 在OpenGL中，Face Culling（面剔除）是一种优化技术，用于减少需要绘制的多边形面，从而提高渲染性能。
     * 通过剔除不需要绘制的面（通常是不可见的面），我们可以减少图形处理单元（GPU）的负载，提高帧率和渲染效率。
     * Face Culling 通过利用物体的面法向量和视图方向的关系来决定哪些面是不可见的，并将其剔除。
     * 一个多边形面在3D空间中由多个顶点组成，根据这些顶点的连接顺序（顺时针或逆时针），可以确定该面是朝向观察者还是背向观察者。

     * 工作原理
     *  Face Culling 的工作原理基于物体表面的法向量和视点方向的关系。以下是Face Culling的具体过程：
     *   确定面朝向：OpenGL通过顶点的绘制顺序来判断面的朝向。逆时针顶点顺序定义正面，顺时针顺序定义背面（或反之，取决于glFrontFace的设置）。
     *   剔除背面：在默认设置下，背向视点的面将被剔除，即在OpenGL进行光栅化之前，这些面不会被送入渲染管线，从而节省了计算资源。
    */

    /*
     * 在OpenGL中，使用以下函数来启用面剔除，默认面剔除是关闭的。
    */
    glEnable(GL_CULL_FACE);

    /*
     * 选择剔除哪些面（正面或反面）。OpenGL默认剔除的是背面。通过以下函数设置剔除的面。
    */
    glCullFace(GL_FRONT); // 剔除正面
    // glCullFace(GL_BACK);           // 剔除背面（默认）
    // glCullFace(GL_FRONT_AND_BACK); // 剔除正面和背面（通常用于调试）

    /*
     * OpenGL通过顶点的连接顺序来区分多边形的正面和背面。可以使用以下函数设置正面是顺时针还是逆时针。
    */
    // glFrontFace(GL_CCW); // 默认，逆时针（Counter-Clockwise）为正面
    // glFrontFace(GL_CW);  // 顺时针（Clockwise）为正面

    Shader &shader = mesh->GetShader();
    UpdateViewMatrix(shader, true);
    UpdateProjectionMatrix(shader);
    mesh->Draw();
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