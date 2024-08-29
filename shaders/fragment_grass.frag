#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D texture0;

void main()
{
    vec4 color = texture(texture0, texCoord);

    // 在OpenGL的着色器代码中，当 discard 语句执行时，后续的代码语句将不会再执行。
    // 这是因为 discard 语句会立即终止当前片元的处理，并且该片元不会被写入到帧缓冲(包括颜色缓冲、深度缓冲)中，也不会进行进一步的计算或处理。
    if (color.a < 0.01) discard;

    FragColor = color;
}
