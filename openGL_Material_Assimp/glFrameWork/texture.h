#pragma once
#include "core.h"

//��ȡͼƬ���ⲿ��
struct Texture {
    unsigned int id; //��һ������Ԫ
    std::string type;
    aiString path;  // ���Ǵ��������·������������������бȽ�
};