#include "Cube.h"

Cube::Cube(Shader &shader, float radius) : Mesh(&shader), radius(radius)
{
    std::vector<GLfloat> vertices = {
        // Positions          // Normals           // Texture2D Coords
        // 前面
        -radius, -radius, radius, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //
        radius, -radius, radius, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  //
        radius, radius, radius, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   //
        -radius, radius, radius, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  //

        // 后面
        -radius, -radius, -radius, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, //
        -radius, radius, -radius, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  //
        radius, radius, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,   //
        radius, -radius, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  //

        // 左面
        -radius, radius, radius, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   //
        -radius, radius, -radius, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  //
        -radius, -radius, -radius, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, //
        -radius, -radius, radius, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  //

        // 右面
        radius, radius, radius, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   //
        radius, -radius, radius, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  //
        radius, -radius, -radius, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, //
        radius, radius, -radius, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  //

        // 上面
        -radius, radius, -radius, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //
        -radius, radius, radius, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  //
        radius, radius, radius, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   //
        radius, radius, -radius, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  //

        // 下面
        -radius, -radius, -radius, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, //
        radius, -radius, -radius, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  //
        radius, -radius, radius, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   //
        -radius, -radius, radius, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f   //
    };

    std::vector<GLuint> indices = {
        0,  1,  2,  2,  3,  0,  // 前面
        4,  5,  6,  6,  7,  4,  // 后面
        8,  9,  10, 10, 11, 8,  // 左面
        12, 13, 14, 14, 15, 12, // 右面
        16, 17, 18, 18, 19, 16, // 上面
        20, 21, 22, 22, 23, 20  // 下面
    };

    SetupMesh(vertices, indices, VertexAttributePresets::GetPosNormalTexLayout());
}
