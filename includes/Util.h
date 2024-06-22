#pragma once

#include "glad/glad.h"

class Util
{
  private:
    Util();

  public:
    // 删除复制构造函数和赋值操作符
    Util(const Util &) = delete;
    Util &operator=(const Util &) = delete;
    ~Util();

    GLenum CheckOpenGLError();

    // 获取单例实例
    static Util &getInstance();
};