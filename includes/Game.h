#pragma once

#include "Scene.h"
#include "GLFW/glfw3.h"

class Game
{
  private:
    Game();

    GLFWwindow *window;

    Scene scene;

    void SetupWindowHint() const;

    void SetupGLDebugContext() const;

    void PrintGPUInfo() const;

    void PrintOpenGLVersion() const;

    void QueryDefaultFramebufferInfos() const;

  public:
    // 删除复制构造函数和赋值操作符
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;
    ~Game();

    // 获取单例实例
    static Game &getInstance();

    bool Init(const char *title, int width, int height);

    void Draw();

    void Run();

    void On_Key_W_Press();
    void On_Key_S_Press();
    void On_Key_A_Press();
    void On_Key_D_Press();
    void On_Mouse_Move(double xpos, double ypos);
    void On_Mouse_Scroll(double xoffset, double yoffset);
};