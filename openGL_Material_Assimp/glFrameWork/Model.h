#pragma once 
#include "Mesh.h"

#define STBI_WINDOWS_UTF8


class Model
{
public:
    /*  函数   */
    Model(char* path)
    {
        std::cout << "收到模型路径:" << path <<  std::endl;
        loadModel(path);
    }
    void Draw(Shader shader);
private:
    /*  模型数据  */
    std::vector<Mesh> meshes;
    static std::vector<Texture> textures_loaded; //加载过的材质
    std::string directory;
    /*  函数   */
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};