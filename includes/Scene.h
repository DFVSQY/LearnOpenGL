#pragma once

#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include <vector>
#include "Camera.h"

class Scene
{
  private:
    std::vector<Mesh *> m_meshes;
    std::vector<Shader *> m_shaders;
    std::vector<Texture *> m_textures;
    std::vector<Material *> m_materials;

    Camera m_camera;

    float m_camSpeed;
    float m_lastFrameTime;
    float m_deltaTime;

    double m_lastCursorPosX;
    double m_lastCursorPosY;
    double m_cursorSensitivity;
    double m_captureCursor;

    void AddMesh(Mesh *mesh);
    void AddShader(Shader *shader);
    void AddTexture(Texture *texture);
    void AddMaterial(Material *material);

    Shader *LoadShader(const char *vertextPath, const char *fragmentPath);
    Texture *LoadTexture(const char *texturePath, GLenum format);
    Material *GenMaterial(Shader *shader);

    void InitMVP(Material *material);

    void UpdateModelMatrix(Material *material);
    void UpdateViewMatrix(Material *material);
    void UpdateProjectionMatrix(Material *material);

  public:
    Scene();
    ~Scene();

    // 删除复制构造函数和赋值操作符
    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

    void Init(int width, int height);

    Material *SetupMat_1();
    Mesh *SetupMesh_1(Material *material);

    Material *SetupMat_2();
    Mesh *SetupMesh_2(Material *material);

    Material *SetupMat_3();
    Mesh *SetupMesh_3(Material *material);

    Material *SetupMat_4();
    Mesh *SetupMesh_4(Material *material);

    void Render();

    void MoveCamForward();
    void MoveCamBackward();
    void MoveCamLeft();
    void MoveCamRight();
    void UpdateCamYawAndPitch(double xPos, double yPos);
    void UpdateCamZoom(double yoffset);
};