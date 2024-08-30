#version 330 core

layout(location = 0) in vec3 aPos;

layout(location = 2) in vec2 aTexCoord;

// 传递给片段着色器的纹理坐标
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 scalePos = aPos * 10.0;
    gl_Position = projection * view * model * vec4(scalePos, 1.0);

    texCoord = aTexCoord;
}
