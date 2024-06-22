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

/*
 * 在GLSL（OpenGL Shading Language）中，uniform是一种限定符（qualifier），它用于声明全局变量，
 * 这些变量在着色器的多次执行过程中保持不变，直到从外部程序（如CPU端的OpenGL应用）进行更新。
 * uniform变量通常用来传递在渲染过程中不会改变的值，如变换矩阵、光源参数、纹理采样器等。
 * 特点：
 *  数据传递：uniform变量用于在应用程序和着色器之间传递数据。由于uniform变量在渲染过程中保持不变，它们非常适合用于传递那些不需要频繁更改的数据。
 *  全局访问：uniform变量在同一个着色器程序的不同着色器阶段（如顶点着色器和片段着色器）之间共享，使得它们能够被不同的着色器阶段访问和使用。
 *  效率：由于uniform变量在每次绘制调用中保持不变，使用uniform变量可以减少数据传输的频率，进而提高渲染效率。

 * 不同的着色器程序（ShaderProgram）之间不能直接共享uniform变量的数据。
 * 每个着色器程序都有自己独立的uniform变量集合，它们在各自的着色器程序中是独立的。
*/
// uniform vec4 userColor;

in vec2 texCoord;

/*
 * uniform 关键字：
 *  表示这是一个uniform变量。
 *  uniform变量是从CPU端程序传递到GPU着色器的全局变量。
 *  对于一次绘制调用中的所有顶点和片段，uniform变量保持不变。

 * sampler2D 类型：
 *  这是一个特殊的数据类型，用于表示2D纹理。
 *  sampler2D 实际上是一个不透明的引用，指向一个2D纹理。
 *  它允许着色器访问和采样纹理数据。

 * textureSampler 名称：
 *  这是给这个纹理采样器取的名字。
 *  在着色器中可以使用这个名字来引用和使用这个纹理。

 * 通常与 texture 函数一起使用来采样纹理颜色。
 * 在OpenGL程序中，你可以将一个纹理绑定到这个采样器。

 * OpenGL默认激活GL_TEXTURE0纹理单元。
 * 如果你只使用了一个纹理，并且在着色器中使用了默认的纹理单元（通常是0），那么可能不需要显式绑定。
*/
uniform sampler2D textureSampler;

void main()
{
    // FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);

    // FragColor = userColor;

    // FragColor = vertexColor;

    // FragColor = texture(textureSampler, texCoord);

    FragColor = texture(textureSampler, texCoord) * vertexColor;
}
