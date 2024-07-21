#pragma once

#include "glad/glad.h"
#include <vector>

/*
 * 顶点属性结构体，用于指定顶点属性的格式。
*/
struct VertexAttribute
{
    /*
     * 指定每个顶点属性的分量数量。例如，位置通常有3个分量（x, y, z），颜色可能有4个分量（r, g, b, a）。
    */
    GLint size;

    /*
     * 指定数组中每个组件的数据类型。常用的类型有GL_FLOAT、GL_INT、GL_UNSIGNED_INT等。
    */
    GLenum type;

    /*
     * 如果设置为 GL_TRUE，整数类型的数值在被访问时会被映射到 [0,1] (无符号) 或 [-1,1] (有符号) 范围内。
     * 如果设置为 GL_FALSE，则直接作为浮点数处理。
    */
    GLboolean normalized;

    /*
     * 指定连续顶点属性之间的字节偏移量。如果为 0，则表示顶点属性是紧密排列的（即无间隔）。
     * 这个参数用于在顶点缓冲对象中正确定位每个顶点的属性。
    */
    GLsizei stride;

    /*
     * 指定顶点属性数组中第一个组件的字节偏移量。这个指针是从缓冲对象数据的起始位置开始计算的。
    */
    const void *pointer;
};

/*
 * 顶点属性布局的一些预定义组合。
*/
class VertexAttributePresets
{
  public:
    static const std::vector<VertexAttribute> &GetPosColorTexLayout(); /* 纹理-颜色-纹理坐标 */

    static const std::vector<VertexAttribute> &GetPosColorLayout(); /* 位置-颜色 */

    static const std::vector<VertexAttribute> &GetPosLayout(); /* 位置 */

    static const std::vector<VertexAttribute> &GetPosNormalLayout(); /* 位置-法线 */

    static const std::vector<VertexAttribute> &GetPosNormalTexLayout(); /* 位置-法线-纹理坐标 */

  private:
    // 禁止实例化该类
    VertexAttributePresets() = delete;
};