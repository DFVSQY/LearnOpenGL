#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include <vector>
#include "Camera.h"

class Scene
{
  private:
    std::vector<Mesh *> m_meshes;
    std::vector<Shader *> m_shaders;
    std::vector<Texture *> m_textures;
    std::vector<Shader *> m_materials;

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
    void AddMaterial(Shader *material);

    Shader *LoadShader(const std::string &vertextPath, const std::string &fragmentPath);
    Texture *LoadTexture(const std::string &texturePath, const GLenum format);

    void InitMVP(Shader *material);

    void UpdateModelMatrix(Shader *material);
    void UpdateViewMatrix(Shader *material);
    void UpdateProjectionMatrix(Shader *material);

  public:
    Scene();
    ~Scene();

    // 删除复制构造函数和赋值操作符
    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

    void Init(int width, int height);

    Shader *SetupMat_1();
    Mesh *SetupMesh_1(Shader *material);

    Shader *SetupMat_2();
    Mesh *SetupMesh_2(Shader *material);

    Shader *SetupMat_3();
    Mesh *SetupMesh_3(Shader *material);

    Shader *SetupMat_4();
    Mesh *SetupMesh_4(Shader *material);

    Shader *SetupMat_5();
    Mesh *SetupMesh_5(Shader *material);

    Shader *SetupMat_6();
    Mesh *SetupMesh_6(Shader *material);

    Shader *SetupMat_7();
    Mesh *SetupMesh_7(Shader *material);

    Shader *SetupMat_8();
    Mesh *SetupMesh_8(Shader *material);

    void Render();

    void MoveCamForward();
    void MoveCamBackward();
    void MoveCamLeft();
    void MoveCamRight();
    void UpdateCamYawAndPitch(double xPos, double yPos);
    void UpdateCamZoom(double yoffset);
};