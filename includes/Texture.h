#pragma once

#include "glad/glad.h"

class Texture
{
  private:
    GLuint texture_id;

    GLsizei width, height;

    int channel_num;

  public:
    // 删除复制构造函数和赋值操作符
    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;

    Texture();
    ~Texture();

    bool Init(const char *filePath, GLenum format);

    GLsizei GetWidth() const;
    GLsizei GetHeight() const;

    void Use(int idx);
};