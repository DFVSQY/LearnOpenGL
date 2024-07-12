#include "VertexAttribute.h"
#include "glm/glm.hpp"
#include <cstddef>

struct VertexPCT
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texCoords;
};

/*
 * 位置-颜色-纹理坐标 布局
*/
const std::vector<VertexAttribute> &VertexAttributePresets::GetPosColorTexLayout()
{
    static const std::vector<VertexAttribute> attributes = {
        {3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(VertexPCT)), (void *)offsetof(VertexPCT, position)},
        {3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(VertexPCT)), (void *)offsetof(VertexPCT, color)},
        {2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(VertexPCT)), (void *)offsetof(VertexPCT, texCoords)},
    };
    return attributes;
}

struct VertexPC
{
    glm::vec3 position;
    glm::vec3 color;
};

/*
 * 位置-颜色 布局
*/
const std::vector<VertexAttribute> &VertexAttributePresets::GetPosColorLayout()
{
    static const std::vector<VertexAttribute> attributes = {
        {3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(VertexPC)), (void *)offsetof(VertexPC, position)},
        {2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(VertexPC)), (void *)offsetof(VertexPC, color)},
    };
    return attributes;
}