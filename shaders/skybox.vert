#version 330 core

layout(location = 0) in vec3 aPos;

// 传递给片段着色器的纹理坐标
out vec3 texCoord;

// 去除了平移的视点矩阵
uniform mat4 rotView;

uniform mat4 projection;

void main()
{
    texCoord = aPos;
    gl_Position = projection * rotView * vec4(aPos, 1.0);
}
