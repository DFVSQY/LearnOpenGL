#pragma once

#include "Texture.h"

class Texture2D : public Texture
{
  protected:
    GLsizei width, height;

    int channel_num;

    static Texture2D *white_2d_texture;
    static Texture2D *black_2d_texture;

    bool InnerInit(const char *filePath, GLenum format, GLint wrapMode);

    GLenum GetTextureTarget() const override;

  public:
    Texture2D(const char *filePath, GLenum format = 0, GLint wrapMode = GL_REPEAT);
    ~Texture2D() override;

    GLsizei GetWidth() const;
    GLsizei GetHeight() const;

    static Texture2D *GetWhite2DTexture();
    static Texture2D *GetBlack2DTexture();
};