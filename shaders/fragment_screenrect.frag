#version 330 core

// 输出颜色
out vec4 FragColor;

// 纹理坐标
in vec2 texCoord;

uniform sampler2D texture0;

void main()
{
    // vec4 color = texture(texture0, texCoord);

    // 反转颜色
    // texture(texture0, texCoord) 返回的vec4包含四个分量（R, G, B, A），分别表示红色、绿色、蓝色和透明度。
    // "1.0 -" 操作符对这个返回的vec4进行逐分量运算。实际上，它只对vec4的前三个分量（R, G, B）进行操作，减法运算会对每个颜色分量进行 "1.0 -" 分量值的计算。
    // 这样做的结果是反转颜色值。例如，如果纹理在某个位置的红色分量是0.2，那么反转后的红色分量将是0.8。
    vec4 color = vec4(vec3(1.0 - texture(texture0, texCoord)), 1.0);

    FragColor = color;
}
