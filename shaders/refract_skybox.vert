#version 330 core

layout(location = 0) in vec3 aPos;

layout(location = 1) in vec3 aNormal;

// 传递给片段着色器的纹理坐标
out vec2 texCoord;

// 传递给片段着色器的法向量
out vec3 normal;

// 传递给片段着色器的世界坐标
out vec3 worldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// 法线矩阵，用于将法向量从模型空间转换为世界空间
uniform mat3 normalMatrix;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    worldPos = vec3(model * vec4(aPos, 1.0));
    normal = normalMatrix * aNormal; // 转换法向量
}
