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

// 方向光
struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// 点光源
struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// 聚光灯
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

vec3 calDirLight()
{
    // 计算环境光
    vec3 ambient = dirLight.ambient * vec3(texture(material.diffuse, texCoord).rgb);

    // 计算漫反射颜色
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = dirLight.diffuse * diff * vec3(texture(material.diffuse, texCoord).rgb);

    // 计算镜面反射颜色
    vec3 viewDir = normalize(camPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = dirLight.specular * spec * vec3(texture(material.specular, texCoord).rgb);

    vec3 result = diffuse + ambient + specular;

    return result;
}

vec3 calPointLight() {
    float distance = length(pointLight.position - worldPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));

    // 计算环境光
    vec3 ambient = pointLight.ambient * vec3(texture(material.diffuse, texCoord).rgb);

    // 计算漫反射颜色
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(pointLight.position - worldPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = pointLight.diffuse * diff * vec3(texture(material.diffuse, texCoord).rgb);

    // 计算镜面反射颜色
    vec3 viewDir = normalize(camPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = pointLight.specular * spec * vec3(texture(material.specular, texCoord).rgb);

    vec3 result = diffuse + ambient + specular;
    result *= attenuation;

    return result;
}

vec3 calSpotLight()
{
    vec3 lightDir = normalize(spotLight.position - worldPos);
    float theta = dot(lightDir, normalize(-spotLight.direction));

    // 计算环境光
    vec3 ambient = spotLight.ambient * vec3(texture(material.diffuse, texCoord).rgb);

    // 计算漫反射颜色
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = spotLight.diffuse * diff * vec3(texture(material.diffuse, texCoord).rgb);

    // 计算镜面反射颜色
    vec3 viewDir = normalize(camPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spotLight.specular * spec * vec3(texture(material.specular, texCoord).rgb);

    // spotlight (soft edges)
    float epsilon = (spotLight.cutOff - spotLight.outerCutOff);
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    // attenuation
    float distance = length(spotLight.position - worldPos);
    float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = diffuse + ambient + specular;

    return result;
}

void main()
{
    vec3 dir_light_vec3 = calDirLight();
    vec3 point_light_vec3 = calPointLight();
    vec3 spot_light_vec3 = calSpotLight();

    vec3 result = dir_light_vec3 + point_light_vec3 + spot_light_vec3;

    FragColor = vec4(result, 1.0);
}
