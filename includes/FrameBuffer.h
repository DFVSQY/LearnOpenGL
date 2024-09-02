#pragma once
#include <vector>
#include "glad/glad.h"

class FrameBuffer
{
  public:
    FrameBuffer();

    ~FrameBuffer();

    void Bind() const;

    static void Unbind();

    void AttachTexture(GLenum attachment, GLenum internalFormat, GLenum format, GLenum type, GLsizei width,
                       GLsizei height);

    void AttachRenderBuffer(GLenum attachment, GLenum internalFormat, GLsizei width, GLsizei height);

    bool IsComplete() const;

    GLuint GetTextrueID(int idx) const;
    GLuint GetRenderBufferID(int idx) const;

    void BindTexture(int idx, int textureIdx = 0);

  private:
    GLuint m_fbo;
    std::vector<GLuint> m_textures;
    std::vector<GLuint> m_renderBuffers;
};
