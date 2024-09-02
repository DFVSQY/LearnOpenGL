#version 330 core

// 输出颜色
out vec4 FragColor;

// 纹理坐标
in vec2 texCoord;

uniform sampler2D texture0;

void main()
{
    vec4 color = texture(texture0, texCoord);

    FragColor = color;
}
