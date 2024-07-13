#version 330 core

// 输出颜色
out vec4 FragColor;

// 纹理坐标
in vec2 texCoord;

// 法线
in vec3 normal;

// 世界坐标
in vec3 worldPos;

// 纹理
uniform sampler2D texture0;

// 灯光的位置
uniform vec3 lightPos;

// 灯光颜色
uniform vec3 lightColor;

// 摄像机位置
uniform vec3 camPos;

void main()
{
    // 用灯光颜色乘以一个系数模拟环境光
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // 计算漫反射颜色
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - worldPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // 计算镜面反射颜色
    float specularStrength = 0.5;
    vec3 viewDir = normalize(camPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (diffuse + ambient + specular) * texture(texture0, texCoord).rgb;

    FragColor = vec4(result, 1.0);
}
