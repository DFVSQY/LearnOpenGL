#include "Scene.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/trigonometric.hpp"
#include <algorithm>
#include <cmath>
#include <vector>
#include "VertexAttribute.h"

Scene::Scene() : m_meshes(), m_shaders(), m_textures(), m_materials(), m_camera()
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

void Scene::Init(int width, int height)
{
    m_lastCursorPosX = (double)width / 2;
    m_lastCursorPosY = (double)height / 2;

    Material *material = SetupMat_2();
    if (!material)
        return;

    SetupMesh_2(material);
}

////////////////////////////////////////////////// 配置渲染用的材质和网格 ///////////////////////////////////////////////

/*
 * 配置渲染用的材质1
*/
Material *Scene::SetupMat_1()
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

    Material *material = GenMaterial(shader);

    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, glm::vec3(0.5f));
    material->SetMat4f("trans", trans);

    material->SetTexture("texture0", texture);
    material->SetTexture("texture1", texture2);

    AddMaterial(material);

    return material;
}

/*
 * 配置渲染用的网格1
*/
Mesh *Scene::SetupMesh_1(Material *material)
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

    Mesh *mesh = new Mesh(vertices_vec, indices_vec, VertexAttributePresets::GetPosColorTexLayout(), material);
    AddMesh(mesh);

    return mesh;
}

/*
 * 配置渲染用的材质2
*/
Material *Scene::SetupMat_2()
{
    // Shader
    Shader *shader = LoadShader("../shaders/vertex_02.glsl", "../shaders/fragment_02.glsl");
    if (!shader)
        return nullptr;

    // 纹理
    Texture *texture = LoadTexture("../textures/wall.jpg", GL_RGB);
    if (!texture)
        return nullptr;

    Material *material = GenMaterial(shader);

    material->SetTexture("texture0", texture);

    AddMaterial(material);

    return material;
}

/*
 * 配置渲染用的网格2
*/
Mesh *Scene::SetupMesh_2(Material *material)
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

    Mesh *mesh = new Mesh(vertices, indices, VertexAttributePresets::GetPosColorTexLayout(), material);
    AddMesh(mesh);

    return mesh;
}

void Scene::InitMVP(Material *material)
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

    material->SetMat4f("model", model);
    material->SetMat4f("view", view);
    material->SetMat4f("projection", projection);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

void Scene::AddMaterial(Material *material)
{
    m_materials.push_back(material);
}

void Scene::Render()
{
    float now_time = glfwGetTime();
    if (m_lastFrameTime <= 0.0f)
        m_lastFrameTime = now_time;

    m_deltaTime = now_time - m_lastFrameTime;
    m_lastFrameTime = now_time;

    for (size_t i = 0; i < m_meshes.size(); ++i)
    {
        Mesh *mesh = m_meshes[i];

        Material *mat = mesh->GetMaterial();
        UpdateModelMatrix(mat);
        UpdateViewMatrix(mat);
        UpdateProjectionMatrix(mat);

        mesh->Draw();
    }
}

void Scene::UpdateModelMatrix(Material *material)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

    material->SetMat4f("model", model);
}

void Scene::UpdateViewMatrix(Material *material)
{
    glm::mat4 view = m_camera.GetViewMatrix();
    material->SetMat4f("view", view);
}

void Scene::UpdateProjectionMatrix(Material *material)
{
    glm::mat4 projection = m_camera.GetProjectionMatrix();
    material->SetMat4f("projection", projection);
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