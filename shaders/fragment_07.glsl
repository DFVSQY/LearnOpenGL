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

// 摄像机位置
uniform vec3 camPos;

// 材质结构体
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

void main()
{
    vec3 lightDir = normalize(light.position - worldPos);
    float theta = dot(lightDir, normalize(-light.direction));

    // 计算环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord).rgb);

    if (theta > light.cutOff) // 聚光灯范围内使用光照点亮颜色
    {
        // 计算漫反射颜色
        vec3 norm = normalize(normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord).rgb);

        // 计算镜面反射颜色
        vec3 viewDir = normalize(camPos - worldPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord).rgb);

        vec3 result = diffuse + ambient + specular;

        FragColor = vec4(result, 1.0);
    }
    else // 聚光灯范围外，使用环境光让场景在聚光之外时不至于完全黑暗
    {
        // FragColor = vec4(ambient, 1.0);
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}
