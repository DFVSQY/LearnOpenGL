#version 330 core

layout(location = 0) in vec3 aPos;

/*
 * 立方体贴图采样基于方向： 立方体贴图（Cube Map）的采样主要依赖传入的方向向量，而不是它的长度。
 * OpenGL 中的 texture() 函数在采样立方体贴图时只关注传入的纹理坐标向量的方向，因此从理论上来说，传入的向量长度并不会影响最终采样到的纹理颜色。
 * 换句话说，如果将 aPos 乘以一个缩放系数（如 aPos * scale），虽然会改变向量的长度，
 * 但由于立方体贴图采样只关心方向，这个缩放不会直接影响方向，所以在理想条件下，采样结果应保持不变。
*/
out vec3 texCoord;

// 去除了平移的视点矩阵
uniform mat4 rotView;

uniform mat4 projection;

void main()
{
    // 直接将天空盒的顶点坐标作为纹理采样坐标是可以的，且是渲染天空盒时的常用做法。
    // 这种方式利用了立方体映射（Cube Mapping）的原理，使得每个顶点的方向向量可以直接用来采样立方体贴图。
    // 天空盒的网格通常是一个立方体，顶点的坐标范围在 [-1, 1] 之间。
    // 由于这些顶点坐标正好可以表示立方体中的方向向量，因此可以直接将顶点坐标 aPos 作为采样立方体贴图的纹理坐标。
    texCoord = aPos;
    gl_Position = projection * rotView * vec4(aPos, 1.0);
}
