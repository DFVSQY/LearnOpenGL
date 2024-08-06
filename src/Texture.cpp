#include "Texture.h"
#include "stb_image.h"
#include "iostream"

Texture *Texture::white_2d_texture = nullptr;
Texture *Texture::black_2d_texture = nullptr;

Texture *Texture::GetWhite2DTexture()
{
    if (white_2d_texture == nullptr)
    {
        white_2d_texture = new Texture();
        white_2d_texture->Init("../textures/Default_White.png", GL_RGBA);
    }
    return white_2d_texture;
}

Texture *Texture::GetBlack2DTexture()
{
    if (black_2d_texture == nullptr)
    {
        black_2d_texture = new Texture();
        black_2d_texture->Init("../textures/Default_Black.png", GL_RGBA);
    }
    return black_2d_texture;
}

Texture::Texture() : texture_id(0), width(0), height(0), channel_num(0)
{
}

Texture::~Texture()
{
    if (texture_id > 0)
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

bool Texture::Init(const char *filePath, GLenum format)
{
    /*
     * 该函数用于在加载纹理时上下翻转图像。

     * 在OpenGL中，进行纹理映射后渲染出来的图像出现上下颠倒的情况是很常见的。这主要是因为OpenGL和大多数图像文件格式在坐标系统上存在差异。

     * 详细解释：
     *  坐标系统差异：
     *    OpenGL纹理坐标系：原点(0,0)在左下角，y轴向上。
     *    大多数图像文件格式：原点(0,0)在左上角，y轴向下。

     * 加载图像时的影响：
     *    当你加载一个图像作为纹理时，通常图像数据是从顶部到底部存储的。但是当你在OpenGL中使用这些数据时，它会从底部开始渲染，导致图像看起来是上下颠倒的。
    */
    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load(filePath, &width, &height, &channel_num, 0);
    if (!data)
    {
        std::cerr << "Texture load failed!" << std::endl;
        return false;
    }

    // 自动确定纹理的格式
    if (format == 0)
    {
        if (channel_num == 1)
            format = GL_RED;
        else if (channel_num == 3)
            format = GL_RGB;
        else if (channel_num == 4)
            format = GL_RGBA;
    }

    /*
     * glGenTextures是OpenGL中用于生成纹理对象名称的函数。
     * 这个函数的主要目的是创建一个或多个唯一的纹理对象标识符,这些标识符可以在后续的纹理操作中使用。
    */
    glGenTextures(1, &texture_id);

    /*
     * glBindTexture函数的作用:
     *  1. 激活纹理对象: 它将一个纹理对象绑定到当前的OpenGL上下文中的活动纹理单元。
     *  2. 设置当前纹理: 绑定后,所有后续的纹理操作都会作用于这个被绑定的纹理对象。
     *  3. 创建新纹理: 如果指定的纹理名称之前没有被使用过,glBindTexture会创建一个新的纹理对象。

     * 函数原型：void glBindTexture(GLenum target, GLuint texture);
     * 参数：
     *  target: 指定纹理的类型,如GL_TEXTURE_2D, GL_TEXTURE_3D等。
     *  texture: 要绑定的纹理对象的名称(unsigned int)。
    */
    glBindTexture(GL_TEXTURE_2D, texture_id);

    /*
     * 设置水平方向（S轴）和垂直方向（T轴）的纹理wrapping方式，此处为重复纹理。

     * 在OpenGL中，纹理坐标通常被归一化为0.0到1.0之间。
     * 当纹理坐标超出0.0到1.0的范围时，纹理wrapping决定了如何处理这些坐标。
     * GL_REPEAT 通常是最高效的包裹模式，因为它可以利用硬件的纹理寻址能力。

     * glTexParameteri 的作用是设置纹理参数。
     * 函数原型：void glTexParameteri(GLenum target, GLenum pname, GLint param);
     * 参数：
     *  target: 指定纹理目标，如GL_TEXTURE_2D, GL_TEXTURE_3D等。
     *  pname: 指定要设置的参数名称。
     *  param: 指定参数值。

     * 这些设置应该在绑定纹理后、加载纹理数据之前进行。
     * 对于每个新的纹理对象，都需要单独设置这些参数。
    */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /*
     * 设置2D纹理的过滤参数，控制纹理在缩小和放大时的采样方式。

     * 纹理缩小过滤（GL_TEXTURE_MIN_FILTER）：
     *  设置为 GL_LINEAR_MIPMAP_LINEAR，这是一种高质量的三线性过滤方式。
     *  它在两个最接近的 mipmap 级别之间进行线性插值，然后在插值结果上再次进行线性插值，提供了最平滑的缩小效果，但也是计算量最大的。
     *  使用 mipmap 需要生成 mipmap 级别（通常通过 glGenerateMipmap 函数）。

     * 纹理放大过滤（GL_TEXTURE_MAG_FILTER）：
     *  设置为 GL_LINEAR，这种方式使用临近的4个纹素进行双线性插值，提供比 GL_NEAREST 更平滑的放大效果。

     * 为什么缩小和放大使用不同的过滤方式：
     *  缩小时通常需要更复杂的过滤来避免混叠。
     *  放大时通常不需要 mipmap，线性过滤就足够了。

     * 这些设置应在绑定纹理后、加载纹理数据之前进行。
    */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /*
     * 该函数用于定义二维纹理图像的数据。
     * 将图像数据从CPU内存传输到GPU内存。
     * 可能在传输过程中进行格式转换（如从RGB到RGBA）。
     * 在GPU上分配存储空间来存储纹理数据。
     * 这是一个相对昂贵的操作，特别是对于大纹理。

     * 函数原型：void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data);
     * 参数：
     *  target: 指定纹理目标，如GL_TEXTURE_2D, GL_TEXTURE_3D等。
     *  level: 指定纹理的mipmap级别，0是基本级别，大于0的值用于mipmap级别（也可以手动为minmap指定纹理数据）。
     *  internalformat: 指定纹理在GPU中的存储格式，常见值包括GL_RGB, GL_RGBA, GL_RGB8, GL_RGBA8等，更高精度的格式如GL_RGB16F用于HDR纹理。
     *  width: 指定纹理图像的宽度（以像素为单位）。
     *  height: 指定纹理图像的高度（以像素为单位）。
     *  border: 历史遗留参数，必须设为0。
     *  format: 指定提供的像素数据的格式，常见值有GL_RGB, GL_RGBA, GL_RED等。
     *  type: 指定提供的像素数据的数据类型，常见值有GL_UNSIGNED_BYTE, GL_FLOAT等。
     *  data: 指向包含纹理图像数据的内存缓冲区的指针，如果为NULL，则分配存储空间但不初始化它。

     * 注意事项：
     *  调用后，OpenGL会复制数据，因此可以释放原始数据。
     *  某些硬件可能要求纹理尺寸是2的幂（如256x256, 512x512等）。
     *  internalFormat, format, 和 type 需要相互兼容。
     *  对于频繁更新的纹理，考虑使用glTexSubImage2D。
     *  使用glGetError检查可能的错误。
     *  在调用此函数之前，确保正确的纹理已被绑定。
    */
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    /*
     * 该函数用于为当前绑定的纹理自动生成完整的mipmap链。

     * Mipmap 是纹理的一系列预计算的缩小版本，用于提高渲染速度和减少混叠（aliasing）效果，通常只在纹理缩小时使用，放大不使用 mipmap。
     * 每个级别的尺寸是上一级的一半，直到1x1像素，从原始纹理开始，逐步创建更小的版本，使用高质量的缩小算法（通常是线性过滤）。
     * 生成mipmap需要一定的计算时间，但通常比在渲染时动态缩小纹理更高效，增加了约33%的纹理内存使用，额外内存用于存储所有mipmap级别。

     * 函数原型：void glGenerateMipmap(GLenum target);
     *  target: 指定纹理目标，如GL_TEXTURE_2D, GL_TEXTURE_3D等。

     * 注意事项：
     *  在调用glGenerateMipmap之前，必须已经为基本级别（level 0）定义了纹理数据。
     *  确保在调用时正确的纹理已被绑定。
     *  对于非2的幂大小的纹理，某些OpenGL实现可能不支持mipmap生成。
     *  要使用生成的mipmap，需要设置适当的纹理过滤模式（如GL_LINEAR_MIPMAP_LINEAR）。
     *  如果需要更精细的控制，可以手动为每个mipmap级别调用glTexImage2D。
     *  对于频繁更新的纹理，重复调用此函数可能影响性能。
     *  对于某些压缩纹理格式，可能需要手动提供所有mipmap级别。
    */
    glGenerateMipmap(GL_TEXTURE_2D);

    // 解除绑定纹理
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

    glBindTexture(GL_TEXTURE_2D, texture_id);
}
