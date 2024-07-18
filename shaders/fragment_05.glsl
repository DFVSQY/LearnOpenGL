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
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

void main()
{
    // 计算环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord).rgb);

    // 计算漫反射颜色
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-light.direction);
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
