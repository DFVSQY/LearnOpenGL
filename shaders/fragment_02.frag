#version 330 core

// 输出颜色
out vec4 FragColor;

// 顶点颜色
in vec4 vertexColor;

// 纹理坐标
in vec2 texCoord;

// 纹理
uniform sampler2D texture0;

void main()
{
    FragColor = texture(texture0, texCoord) * vertexColor;
}
