#version 330 core

layout(location = 0) in vec3 aPos;

layout(location = 1) in vec3 aColor;

layout(location = 2) in vec2 aTexCoord;

// 传递给片段着色器的颜色
out vec4 vertexColor;

// 传递给片段着色器的纹理坐标
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    vertexColor = vec4(aColor, 1.0);

    texCoord = aTexCoord;
}
