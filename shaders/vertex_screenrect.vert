#version 330 core

layout(location = 0) in vec3 aPos;

layout(location = 2) in vec2 aTexCoord;

// 传递给片段着色器的纹理坐标
out vec2 texCoord;

void main()
{
    vec3 scale_pos = aPos * 2.0;
    gl_Position = vec4(scale_pos, 1.0);

    texCoord = aTexCoord;
}
