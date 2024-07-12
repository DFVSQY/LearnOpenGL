#include "VertexAttribute.h"
#include "glm/glm.hpp"
#include <cstddef>

struct VertexPCT
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texCoords;
};

// 定义 getPositionColorTexCoords 方法
const std::vector<VertexAttribute> &VertexAttributePresets::getPositionColorTexCoords()
{
    static const std::vector<VertexAttribute> attributes = {
        {3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(VertexPCT)), (void *)offsetof(VertexPCT, position)},
        {3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(VertexPCT)), (void *)offsetof(VertexPCT, color)},
        {2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(VertexPCT)), (void *)offsetof(VertexPCT, texCoords)},
    };
    return attributes;
}

struct VertexPT
{
    glm::vec3 position;
    glm::vec2 texCoords;
};

// 定义 getPositionTexCoords 方法
const std::vector<VertexAttribute> &VertexAttributePresets::getPositionTexCoords()
{
    static const std::vector<VertexAttribute> attributes = {
        {3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(VertexPT)), (void *)offsetof(VertexPT, position)},
        {2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(VertexPT)), (void *)offsetof(VertexPT, texCoords)},
    };
    return attributes;
}

struct VertexPTN
{
    glm::vec3 position;
    glm::vec2 texCoords;
    glm::vec3 normal;
};

// 定义 getPositionTexCoordsNormals 方法
const std::vector<VertexAttribute> &VertexAttributePresets::getPositionTexCoordsNormals()
{
    static const std::vector<VertexAttribute> attributes = {
        {3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(VertexPTN)), (void *)offsetof(VertexPTN, position)},
        {2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(VertexPTN)), (void *)offsetof(VertexPTN, texCoords)},
        {3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(VertexPTN)), (void *)offsetof(VertexPTN, normal)},
    };
    return attributes;
}