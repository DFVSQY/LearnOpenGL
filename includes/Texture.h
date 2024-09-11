#pragma once

#include "glad/glad.h"

class Texture
{
  protected:
    GLuint texture_id;

  public:
    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;

    Texture();
    virtual ~Texture();

    void Use(int idx) const;
    bool IsValidTexture() const;
};