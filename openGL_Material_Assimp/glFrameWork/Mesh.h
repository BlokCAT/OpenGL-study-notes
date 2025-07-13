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
    /*  ��������  */
    std::vector<Vertex> vertices;  //�кܶඥ�㣬ÿһ��������λ����Ϣ�����ߣ�UV����
    std::vector<unsigned int> indices;
    std::vector<Texture> textures; //���������������һ������Ԫ

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);


    void Draw(Shader& shader);  //����SHADER����Ⱦ�������mesh , 
    //����ɫ�������������п����������ڻ���֮ǰ����һЩuniform���������Ӳ�����������Ԫ����

private:

    unsigned int VAO, VBO, EBO;
    void setupMesh();//��ʼ������

};