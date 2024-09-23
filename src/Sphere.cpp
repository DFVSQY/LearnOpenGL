#include "Sphere.h"
#include "Mesh.h"
#include "Texture.h"
#include "VertexAttribute.h"
#include <vector>
#include <cmath>

Sphere::Sphere(Shader &shader, float radius, unsigned int longitudeDivs, unsigned int latitudeDivs)
    : Mesh(&shader), radius(radius), longitude_divs(longitudeDivs), latitude_divs(latitudeDivs)
{
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    GenMeshData(vertices, indices);

    SetupMesh(vertices, indices, VertexAttributePresets::GetPosNormalTexLayout());
}

Sphere::~Sphere()
{
}

/*
 * 在OpenGL中构建一个球体网格，并包含顶点位置和法线属性，可以通过将球体表面离散化为三角形网格来实现。
 * 通常使用 球面坐标系 将球体的表面参数化，然后将其转换为 笛卡尔坐标系 中的顶点位置，同时利用球体表面特性直接获取法线信息。

 * 参数化球面：
 * 使用两个参数：经度 (longitude) 和 纬度 (latitude)。
 * 经度 (𝜃) 范围为 0 到 2π。
 * 纬度 (𝜙) 范围为 −𝜋/2 到 𝜋/2。
 * 球的半径设为 r。
 * 球面坐标转换为笛卡尔坐标： 球面上的点的笛卡尔坐标可以通过以下公式计算：
 * 	𝑥 = 𝑟 * cos(𝜃) * cos(𝜙)
 * 	𝑦 = 𝑟 * sin(𝜙)
 * 	𝑧 = 𝑟 * sin⁡(𝜃) * cos(𝜙)
 * 其中，𝜃表示经度，𝜙表示纬度。

 * 生成球体网格：
 * 	为了构建一个球体网格，可以使用两个循环来遍历经度和纬度，分别采样这些参数值，生成相应的顶点。将相邻的四个点组成两个三角形来构建球体网格。

 * 法线计算：
 * 	对于每个顶点，球体的法线可以直接使用归一化后的顶点位置，因为球体的法线向量就是该点相对于球心的方向向量（在单位球上，法线就是顶点位置的归一化形式）。

 * 纹理坐标计算：
 *  要在球体网格中添加 纹理坐标，可以基于经度 (𝜃) 和纬度 (𝜙) 的参数化，将它们映射到 2D 纹理坐标系。
 *  对于球体的纹理坐标 (𝑢,𝑣)，可以简单地将经度和纬度的范围映射到纹理坐标的范围 [0,1]。

 * 𝑢对应经度𝜃，范围从 0 到 2π，可以映射到 [0, 1]
 * v对应纬度 𝜙，范围从 -𝜋/2 到 𝜋/2，可以映射到 [0,1]。
*/
void Sphere::GenMeshData(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices)
{
    const float pi = 3.14159265359f;
    const float half_pi = pi / 2.0f;

    // 遍历纬度和经度
    for (unsigned int lat = 0; lat <= latitude_divs; ++lat)
    {
        float phi = -half_pi + pi * float(lat) / float(latitude_divs); // 纬度从 -π/2 到 π/2
        float cos_phi = cos(phi);
        float sin_phi = sin(phi);

        for (unsigned int lon = 0; lon <= longitude_divs; ++lon)
        {
            float theta = 2.0f * pi * float(lon) / float(longitude_divs); // 经度从 0 到 2π
            float cos_theta = cos(theta);
            float sin_theta = sin(theta);

            // 计算顶点位置
            float x = radius * cos_theta * cos_phi;
            float y = radius * sin_phi;
            float z = radius * sin_theta * cos_phi;

            // 法线
            float nx = cos_theta * cos_phi;
            float ny = sin_phi;
            float nz = sin_theta * cos_phi;

            // 计算纹理坐标
            float u = float(lon) / float(longitude_divs); // 经度方向的纹理坐标
            float v = float(lat) / float(latitude_divs);  // 纬度方向的纹理坐标

            // 位置
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // 法线
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);

            // 纹理坐标
            vertices.push_back(u);
            vertices.push_back(v);
        }
    }

    // 生成索引，构建三角形
    for (unsigned int lat = 0; lat < latitude_divs; ++lat)
    {
        for (unsigned int lon = 0; lon < longitude_divs; ++lon)
        {
            unsigned int first = lat * (longitude_divs + 1) + lon;
            unsigned int second = first + longitude_divs + 1;

            // 每个四边形由两个三角形构成
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
}