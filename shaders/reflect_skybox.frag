#version 330 core

// 输出颜色
out vec4 FragColor;

// 法线
in vec3 normal;

// 世界坐标
in vec3 worldPos;

// 摄像机位置
uniform vec3 camPos;

uniform samplerCube skybox;

void main()
{
    // 视线看向物体的方向
    vec3 I = normalize(worldPos - camPos);

    // 计算反射方向
    vec3 R = reflect(I, normalize(normal));

    // 根据反射方向采样天空盒
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
