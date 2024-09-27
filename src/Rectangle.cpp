#include "Rectangle.h"

Rectangle::Rectangle(Shader &shader, GLfloat width, GLfloat height) : Mesh(&shader), width(width), height(height)
{
    std::vector<GLfloat> vertices = {
        // Positions                   // Normals        // Texture Coords
        -width / 2, -height / 2, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom-left
        width / 2,  -height / 2, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // Bottom-right
        width / 2,  height / 2,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Top-right
        -width / 2, height / 2,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f  // Top-left
    };

    std::vector<GLuint> indices = {
        0, 2, 1, // First triangle
        0, 3, 2  // Second triangle
    };
    SetupMesh(vertices, indices, VertexAttributePresets::GetPosNormalTexLayout());
}

Rectangle::~Rectangle()
{
}