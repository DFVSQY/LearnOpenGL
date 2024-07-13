#include "VertexAttribute.h"
#include "glm/glm.hpp"
#include <cstddef>
#include <vector>

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

struct VertexP
{
    glm::vec3 position;
};

/*
 * 位置 布局
*/
const std::vector<VertexAttribute> &VertexAttributePresets::GetPosLayout()
{
    static const std::vector<VertexAttribute> attributes = {
        {3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(VertexP)), (void *)offsetof(VertexP, position)},
    };
    return attributes;
}