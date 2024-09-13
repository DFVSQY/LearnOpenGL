#version 330 core

// 输出颜色
out vec4 FragColor;

// 纹理坐标
in vec3 texCoord;

// 纹理
uniform samplerCube cube_map;

void main()
{
    FragColor = texture(cube_map, texCoord);
}
