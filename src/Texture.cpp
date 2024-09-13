#include "Texture.h"
#include <iostream>

Texture::Texture() : texture_id(0)
{
}

Texture::~Texture()
{
    if (IsValidTexture())
    {
        /*
         * glDeleteTextures 是 OpenGL 中用于删除纹理对象的函数。它的作用是释放先前由 glGenTextures 创建的纹理对象及其相关资源。
         * 当调用 glDeleteTextures 时，OpenGL 会将指定的纹理对象标记为删除。
         * 实际的删除动作不会立即发生，直到所有对该纹理对象的引用（例如在着色器中使用或绑定到任何目标）都被解除。
         * 这样做可以避免在实际应用过程中由于突然删除纹理导致的问题。

         * 函数原型：void glDeleteTextures(GLsizei n, const GLuint *textures);
         *  n：要删除的纹理对象的数量。
         *  textures：一个包含要删除的纹理对象名称（ID）的数组。
        */
        glDeleteTextures(1, &texture_id);
        texture_id = 0;
    }
}

void Texture::Use(int idx) const
{
    /*
     * 函数原型：void glActiveTexture(GLenum texture);
     *  texture: 指定纹理单元， GL_TEXTURE0 到 GL_TEXTURE31 之间的值，可使用 GL_TEXTURE0 + idx 形式指定。

     * 激活纹理单元：
     *  OpenGL 支持多个纹理单元，glActiveTexture 用于选择当前活动的纹理单元。
     *  纹理单元是 GPU 中用于存储和应用纹理的硬件资源。

     * 纹理单元数量：
     *  OpenGL 保证至少支持 16 个纹理单元，但许多现代 GPU 支持 32 个或更多。

     * 默认状态：
     *  如果从未调用 glActiveTexture，默认激活的是 GL_TEXTURE0。

     * 与 glBindTexture 的关系：
     *  glActiveTexture 选择当前活动的纹理单元。
     *  随后的 glBindTexture 调用会将纹理绑定到这个活动的纹理单元。

     * 在着色器中的使用：
     *  着色器中的采样器变量对应于纹理单元，通常采样器变量对应的纹理单元可通过OpenGL程序的 glUniform1i 函数指定。

     * 多重纹理：
     *  允许在一个绘制调用中使用多个纹理。
     *  每个纹理可以绑定到不同的纹理单元。
    */
    glActiveTexture(GL_TEXTURE0 + idx);

    GLenum target = GetTextureTarget();
    glBindTexture(target, texture_id);
}

bool Texture::IsValidTexture() const
{
    return texture_id > 0;
}