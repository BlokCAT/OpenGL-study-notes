#pragma once
#include "core.h"
#include <string>

class Texture {
public:
	Texture(const std::string& path, unsigned int unit);
	~Texture();
	void bind();
	int getWidth() { return this->mWidth; }
	int getHeight() { return this->mHeight; }
	int getUnit() { return this->mUnit; }
private:
	GLuint mTexture{ 0 };
	int mWidth{ 0 };
	int mHeight{ 0 };
	int mUnit{ 0 }; // �������Ŀǰ������һ������Ԫ��
	int mChannels{ 0 };
};