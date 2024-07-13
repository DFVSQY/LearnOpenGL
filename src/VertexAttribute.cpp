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

struct VertextPN
{
    glm::vec3 position;
    glm::vec3 normal;
};

/*
 * 位置-法线 布局
*/
const std::vector<VertexAttribute> &VertexAttributePresets::GetPosNormalLayout()
{
    static const std::vector<VertexAttribute> attributes = {
        {3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(VertextPN)), (void *)offsetof(VertextPN, position)},
        {3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(VertextPN)), (void *)offsetof(VertextPN, normal)}};
    return attributes;
}

struct VertexPNT
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

/*
 * 位置-法线-纹理坐标 布局
*/
const std::vector<VertexAttribute> &VertexAttributePresets::GetPosNormalTexLayout()
{
    static const std::vector<VertexAttribute> attributes = {
        {3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(VertexPNT)), (void *)offsetof(VertexPNT, position)},
        {3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(VertexPNT)), (void *)offsetof(VertexPNT, normal)},
        {2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(VertexPNT)), (void *)offsetof(VertexPNT, texCoords)},
    };
    return attributes;
}