#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // 稍微放大一点
    vec3 scalePos = aPos * 1.02;

    gl_Position = projection * view * model * vec4(scalePos, 1.0);
}
