#include "Texture.h"
#include "stb_image.h"
#include "iostream"

Texture::Texture() : texture_id(0), width(0), height(0), channel_num(0)
{
}

Texture::~Texture()
{
}

bool Texture::Init(const char *filePath, GLenum format)
{
    unsigned char *data = stbi_load(filePath, &width, &height, &channel_num, 0);
    if (!data)
    {
        std::cerr << "Texture load failed!" << std::endl;
        return false;
    }

    glGenTextures(1, &texture_id);

    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    return true;
}

GLsizei Texture::GetWidth() const
{
    return width;
}

GLsizei Texture::GetHeight() const
{
    return height;
}

void Texture::Use()
{
    glBindTexture(GL_TEXTURE_2D, texture_id);
}
