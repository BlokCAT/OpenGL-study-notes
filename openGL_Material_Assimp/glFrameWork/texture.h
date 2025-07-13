#pragma once
#include "core.h"

//读取图片的外部库
struct Texture {
    unsigned int id; //哪一个纹理单元
    std::string type;
    aiString path;  // 我们储存纹理的路径用于与其它纹理进行比较
};