#include "TextureCubeMap.h"
#include "Texture.h"
#include "stb_image.h"
#include <iostream>

TextureCubeMap::TextureCubeMap(const std::vector<const char *> &faces)
{
    InnerInit(faces);
}

TextureCubeMap::~TextureCubeMap()
{
}

bool TextureCubeMap::InnerInit(const std::vector<const char *> &faces)
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

    /*
	 * 分配一个未使用的纹理对象名称（即纹理ID），用于后续的纹理操作。
	*/
    glGenTextures(1, &texture_id);

    /*
	 * 将指定的纹理对象绑定到当前的纹理目标（这里是GL_TEXTURE_CUBE_MAP）。一旦绑定，后续的纹理操作都将作用于当前绑定的纹理对象。
	 * GL_TEXTURE_CUBE_MAP是一种特殊的纹理类型，由6个面（通常为正x、负x、正y、负y、正z、负z）组成，用于创建三维环境贴图或者天空盒。
	 * 	GL_TEXTURE_CUBE_MAP_POSITIVE_X (右面)
	 * 	GL_TEXTURE_CUBE_MAP_NEGATIVE_X (左面)
	 * 	GL_TEXTURE_CUBE_MAP_POSITIVE_Y (上面)
	 * 	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y (下面)
	 * 	GL_TEXTURE_CUBE_MAP_POSITIVE_Z (前面)
	 * 	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z (后面)
	*/
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

    /*
	 * 设置纹理的过滤参数，控制纹理在缩小和放大时的采样方式。
	*/
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    /*
	 * 设置x方向（S轴）和y方向（T轴）和z方向（R轴）的纹理wrapping方式
	*/
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    unsigned char *data = nullptr;
    int width, height, channel_num;
    for (unsigned int idx = 0; idx < faces.size(); idx++)
    {
        data = stbi_load(faces[idx], &width, &height, &channel_num, 0);
        if (!data)
        {
            std::cerr << "Cubemap texture failed to load at path: " << faces[idx] << std::endl;
            return false;
        }

        int format = channel_num == 4 ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + idx, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return true;
}