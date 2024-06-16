#pragma once
#include "GLFW/glfw3.h"

class Game
{
  private:
    Game();

    GLFWwindow *window;

    void SetupWindowHint();

  public:
    // 删除复制构造函数和赋值操作符
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

    // 获取单例实例
    static Game &getInstance();

    bool Init(const char *title, int width, int height);

    void Draw();

    void Run();
};