#version 330 core

// 输出颜色
out vec4 FragColor;

// 纹理坐标
in vec2 texCoord;

uniform sampler2D texture0;

// 可以自定义数值
const float offset = 1.0 / 300.0;

void main()
{
    vec4 color;

    // 正常的纹理采样渲染
    {
        // color = texture(texture0, texCoord);
    }

    // 反转颜色
    // texture(texture0, texCoord) 返回的vec4包含四个分量（R, G, B, A），分别表示红色、绿色、蓝色和透明度。
    // "1.0 -" 操作符对这个返回的vec4进行逐分量运算。实际上，它只对vec4的前三个分量（R, G, B）进行操作，减法运算会对每个颜色分量进行 "1.0 -" 分量值的计算。
    // 这样做的结果是反转颜色值。例如，如果纹理在某个位置的红色分量是0.2，那么反转后的红色分量将是0.8。
    {
        // color = vec4(vec3(1.0 - texture(texture0, texCoord)), 1.0);
    }

    // 灰度化原理
    // 灰度值通常通过加权平均法计算，这种方法根据人眼对不同颜色的敏感度，给红、绿、蓝分量赋予不同的权重。最常用的加权公式是：
    // Grayscale = 0.299 × R + 0.587 × G + 0.114 × B
    // 这里，红色、绿色和蓝色分量分别乘以相应的权重，然后将它们相加得到灰度值。这个公式反映了人眼对绿色最敏感，对红色次之，对蓝色最不敏感的特点。
    {
        // vec4 tex_color = texture(texture0, texCoord);
        // color = vec4(vec3(0.2126 * tex_color.r + 0.7152 * tex_color.g + 0.0722 * tex_color.b), tex_color.a);
    }

    // Kernel Effect 之锐化（Sharpen）
    {
        // vec2 offsets[9] = vec2[](
        //         vec2(-offset, offset), // 左上
        //         vec2(0.0f, offset), // 正上
        //         vec2(offset, offset), // 右上
        //         vec2(-offset, 0.0f), // 左
        //         vec2(0.0f, 0.0f), // 中
        //         vec2(offset, 0.0f), // 右
        //         vec2(-offset, -offset), // 左下
        //         vec2(0.0f, -offset), // 正下
        //         vec2(offset, -offset) // 右下
        //     );

        // // 大部分核将所有的权重加起来之后都应该会等于1，如果它们加起来不等于1，这就意味着最终的纹理颜色将会比原纹理值更亮或者更暗了。
        // float kernel[9] = float[](
        //         -1, -1, -1,
        //         -1, 9, -1,
        //         -1, -1, -1
        //     );

        // // 对片元对应的纹理坐标及其周边进行采样
        // vec3 sampleTex[9];
        // for (int i = 0; i < 9; i++)
        // {
        //     sampleTex[i] = vec3(texture(texture0, texCoord.st + offsets[i]));
        // }

        // // 按照权重混合
        // vec3 col = vec3(0.0);
        // for (int i = 0; i < 9; i++)
        //     col += sampleTex[i] * kernel[i];

        // color = vec4(col, 1.0);
    }

    // Kernel Effect 之模糊（Blur）
    {
        // vec2 offsets[9] = vec2[](
        //         vec2(-offset, offset), // 左上
        //         vec2(0.0f, offset), // 正上
        //         vec2(offset, offset), // 右上
        //         vec2(-offset, 0.0f), // 左
        //         vec2(0.0f, 0.0f), // 中
        //         vec2(offset, 0.0f), // 右
        //         vec2(-offset, -offset), // 左下
        //         vec2(0.0f, -offset), // 正下
        //         vec2(offset, -offset) // 右下
        //     );

        // // 大部分核将所有的权重加起来之后都应该会等于1，如果它们加起来不等于1，这就意味着最终的纹理颜色将会比原纹理值更亮或者更暗了。
        // float kernel[9] = float[](
        //         1.0 / 16, 2.0 / 16, 1.0 / 16,
        //         2.0 / 16, 4.0 / 16, 2.0 / 16,
        //         1.0 / 16, 2.0 / 16, 1.0 / 16
        //     );

        // // 对片元对应的纹理坐标及其周边进行采样
        // vec3 sampleTex[9];
        // for (int i = 0; i < 9; i++)
        // {
        //     sampleTex[i] = vec3(texture(texture0, texCoord.st + offsets[i]));
        // }

        // // 按照权重混合
        // vec3 col = vec3(0.0);
        // for (int i = 0; i < 9; i++)
        //     col += sampleTex[i] * kernel[i];

        // color = vec4(col, 1.0);
    }

    // Kernel Effect 之边缘检测（Edge-detection）
    {
        vec2 offsets[9] = vec2[](
                vec2(-offset, offset), // 左上
                vec2(0.0f, offset), // 正上
                vec2(offset, offset), // 右上
                vec2(-offset, 0.0f), // 左
                vec2(0.0f, 0.0f), // 中
                vec2(offset, 0.0f), // 右
                vec2(-offset, -offset), // 左下
                vec2(0.0f, -offset), // 正下
                vec2(offset, -offset) // 右下
            );

        // 大部分核将所有的权重加起来之后都应该会等于1，如果它们加起来不等于1，这就意味着最终的纹理颜色将会比原纹理值更亮或者更暗了。
        float kernel[9] = float[](
                1, 1, 1,
                1, -8, 1,
                1, 1, 1
            );

        // 对片元对应的纹理坐标及其周边进行采样
        vec3 sampleTex[9];
        for (int i = 0; i < 9; i++)
        {
            sampleTex[i] = vec3(texture(texture0, texCoord.st + offsets[i]));
        }

        // 按照权重混合
        vec3 col = vec3(0.0);
        for (int i = 0; i < 9; i++)
            col += sampleTex[i] * kernel[i];

        color = vec4(col, 1.0);
    }

    FragColor = color;
}
