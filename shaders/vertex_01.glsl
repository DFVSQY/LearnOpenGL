
/*
 * 形式：#version <version> <profile>

 * 这行代码通常放在着色器的开头部分，是编译器指令的一部分，它告诉着色器编译器如何解释和编译这段 GLSL 代码。
 * #version指令用于指定着色器代码所使用的GLSL（OpenGL Shading Language）版本。
 * 这对于确保着色器代码的兼容性非常重要，因为不同版本的GLSL可能会支持不同的特性和语法。

 * #version 330 core的具体含义及作用：
 *  #version：这是一个预处理指令，告诉编译器后面的文本是GLSL版本声明。
 *  330：指定了GLSL的版本号，这里是3.30。版本号通常与OpenGL的版本号相关联，OpenGL 3.3对应GLSL 3.30。
 *  core：指定了使用的配置文件，这里是核心配置文件。core配置文件仅包括OpenGL的核心特性集，没有包含任何弃用的特性。另一个选项是compatibility（兼容性配置文件），它包括核心特性集外的一些旧特性，以支持向后兼容。
*/
#version 330 core

/*
 * layout (location = 0)：指定输入变量的属性位置，这个位置与 OpenGL 程序中绑定的顶点属性位置一致。
 * 在 OpenGL 应用程序中，顶点属性的位置可以通过 glVertexAttribPointer 函数设定，确保其位置与这里的声明一致。

 * in vec3 aPos：声明一个输入变量 aPos，类型是三维向量 (vec3)。这个变量将接收每个顶点的坐标（x, y, z）。

 * 位置属性绑定：在 OpenGL 应用程序中，位置属性通常使用如下代码进行绑定：
 *  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
 *  glEnableVertexAttribArray(0);
 * 这里，index的 0 对应于 layout (location = 0)，表示顶点属性位置。
*/
layout(location = 0) in vec3 aPos;

/*
 * 如果想要从一个着色器向另一个着色器发送数据，我们必须在发送着色器中声明一个输出，并在接收着色器中声明一个类似的输入。
 * 当两边的类型和名称相同时，OpenGL会将这两个变量连接起来，然后就可以在着色器之间发送数据（这是在链接程序对象时完成的）。
*/
out vec4 vertexColor;

/*
 * 每个着色器程序都必须包含的主函数，作为程序的入口点。
*/
void main()
{
    // * gl_Position：这是一个内置的输出变量，表示顶点的最终位置。它是一个四维向量 (vec4)，表示裁剪坐标系中的顶点位置。
    // * 这个值会被传递给下一阶段的图形管线（如几何着色器或光栅化过程）。
    // * vec4(aPos.x, aPos.y, aPos.z, 1.0)：将三维顶点位置 aPos 转换为四维向量。
    // * 第四个分量设置为 1.0，因为在齐次坐标系中，w 分量为 1 表示位置坐标（而不是方向向量）。
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);

    vertexColor = vec4(0.8, 0.8, 1.0, 1.0);
}
