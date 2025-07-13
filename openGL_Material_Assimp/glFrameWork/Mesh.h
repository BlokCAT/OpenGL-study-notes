#pragma once
#include "shader.h"
#include "texture.h"
struct Vertex 
{
    glm::vec3 Position;  
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh
{
public:
    /*  网格数据  */
    std::vector<Vertex> vertices;  //有很多顶点，每一个顶点有位置信息，法线，UV坐标
    std::vector<unsigned int> indices;
    std::vector<Texture> textures; //纹理包含长宽，是哪一个纹理单元

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);


    void Draw(Shader& shader);  //传进SHADER，渲染绘制这个mesh , 
    //将着色器传入网格类中可以让我们在绘制之前设置一些uniform（像是链接采样器到纹理单元）。

private:

    unsigned int VAO, VBO, EBO;
    void setupMesh();//初始化缓冲

};