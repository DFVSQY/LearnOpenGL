#pragma once

#include "glad/glad.h"

class Texture
{
  private:
    GLuint texture_id;

    GLsizei width, height;

    int channel_num;

    static Texture *white_2d_texture;
    static Texture *black_2d_texture;

  public:
    // 删除复制构造函数和赋值操作符
    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;

    Texture();
    ~Texture();

    bool Init(const char *filePath, GLenum format = 0);

    GLsizei GetWidth() const;
    GLsizei GetHeight() const;

    void Use(int idx) const;

    static Texture *GetWhite2DTexture();
    static Texture *GetBlack2DTexture();
};