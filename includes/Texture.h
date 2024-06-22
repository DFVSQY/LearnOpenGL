#pragma once

#include "glad/glad.h"

class Texture
{
  private:
    GLuint texture_id;

    GLsizei width, height;

    int channel_num;

  public:
    Texture();
    ~Texture();

    bool Init(const char *filePath, GLenum format);

    GLsizei GetWidth() const;
    GLsizei GetHeight() const;

    void Use();
};