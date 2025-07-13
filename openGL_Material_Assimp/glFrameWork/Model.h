#pragma once 
#include "Mesh.h"

#define STBI_WINDOWS_UTF8


class Model
{
public:
    /*  ����   */
    Model(char* path)
    {
        std::cout << "�յ�ģ��·��:" << path <<  std::endl;
        loadModel(path);
    }
    void Draw(Shader shader);
private:
    /*  ģ������  */
    std::vector<Mesh> meshes;
    static std::vector<Texture> textures_loaded; //���ع��Ĳ���
    std::string directory;
    /*  ����   */
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};