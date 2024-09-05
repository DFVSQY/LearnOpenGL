#pragma once

#include "glad/glad.h"

class Texture2D
{
  private:
    GLuint texture_id;

    GLsizei width, height;

    int channel_num;

    static Texture2D *white_2d_texture;
    static Texture2D *black_2d_texture;

    bool InnerInit(const char *filePath, GLenum format, GLint wrapMode);

  public:
    // 删除复制构造函数和赋值操作符
    Texture2D(const Texture2D &) = delete;
    Texture2D &operator=(const Texture2D &) = delete;

    Texture2D(const char *filePath, GLenum format = 0, GLint wrapMode = GL_REPEAT);
    ~Texture2D();

    GLsizei GetWidth() const;
    GLsizei GetHeight() const;

    void Use(int idx) const;

    bool IsValidTexture() const;

    static Texture2D *GetWhite2DTexture();
    static Texture2D *GetBlack2DTexture();
};