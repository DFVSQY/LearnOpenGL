#version 330 core

/*
 * out vec4 FragColor：声明一个输出变量 FragColor，类型是四维向量 (vec4)。
 * out 关键字表示这个变量的值将输出到帧缓冲区，并最终决定片段的颜色。

 * vec4：四维向量，通常用于表示 RGBA 颜色值（红、绿、蓝和透明度）。
*/
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
