#version 330 core

/*
 * out vec4 FragColor：声明一个输出变量 FragColor，类型是四维向量 (vec4)。
 * out 关键字表示这个变量的值将输出到帧缓冲区，并最终决定片段的颜色。

 * vec4：四维向量，通常用于表示 RGBA 颜色值（红、绿、蓝和透明度）。
*/
out vec4 FragColor;

/*
 * 在顶点着色器中声明了一个顶点颜色变量作为vec4输出，并在顶点着色器中设置。
 * 在片段着色器中声明了一个类似的顶点颜色输入，由于它们具有相同的类型和名称，因此片段着色器中的顶点颜色与顶点着色器中的顶点颜色相关联。
*/
in vec4 vertexColor;

void main()
{
    // FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);

    FragColor = vertexColor;
}
