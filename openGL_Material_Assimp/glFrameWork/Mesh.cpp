#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}


void Mesh::Draw(Shader& shader)
{
    unsigned int diffuseNr = 1; 
    unsigned int specularNr = 1; 
    for (unsigned int i = 0; i < textures.size(); i++) 
    {
        glActiveTexture(GL_TEXTURE0 + i); // 在绑定之前激活相应的纹理单元
        // 获取纹理序号（diffuse_textureN 中的 N）
        std::string number; 
        std::string name = textures[i].type; 
        if (name == "texture_diffuse") 
            number = std::to_string(diffuseNr++); 
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); 

        shader.setUniformInt(("material." + name + number).c_str(), i); 
        glBindTexture(GL_TEXTURE_2D, textures[i].id); 
    }
    glActiveTexture(GL_TEXTURE0);

    // 绘制网格
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                  // 属性索引（layout(location=0)）
        3,                  // 分量数量（vec3）
        GL_FLOAT,           // 数据类型
        GL_FALSE,           // 是否归一化
        sizeof(Vertex),     // 步长（Stride）：一个Vertex的大小
        (void*)0            // 偏移量：Position在结构体中的偏移
    );

    // 顶点法线（属性索引 1）
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,                  // 属性索引（layout(location=1)）
        3,                  // 分量数量（vec3）
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),     // 步长：仍然是Vertex的大小
        (void*)offsetof(Vertex, Normal)  // 偏移量：Normal的偏移
    );

    // 顶点纹理坐标（属性索引 2）
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,                  // 属性索引（layout(location=2)）
        2,                  // 分量数量（vec2）
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),     // 步长
        (void*)offsetof(Vertex, TexCoords)  // 偏移量：TexCoords的偏移
    );


    glBindVertexArray(0);

}