#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../application/stb_image.h" //读取图片的外部库

Texture::Texture(const std::string& path, unsigned int unit)
{
	mUnit = unit;

	stbi_set_flip_vertically_on_load(true); //翻转y轴

	//读图片
	unsigned char* data = stbi_load(path.c_str(), &mWidth, &mHeight, &mChannels, STBI_rgb_alpha);

	if (data)
	{
		glGenTextures(1, &mTexture);
		glActiveTexture(GL_TEXTURE0 + mUnit);
		glBindTexture(GL_TEXTURE_2D, mTexture);
		//向GPU传递纹理数据,会开辟显存
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		
		//手动设置mipmap级别
		int w = mWidth;
		int h = mHeight;
		for (int level = 0; true; level++)
		{
			glTexImage2D(GL_TEXTURE_2D, level , GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

			if (w == 1 && h == 1)break;

			w = w > 1 ? w / 2 : 1;
			h = h > 1 ? h / 2 : 1;

		}


		//设置过滤方式 , 使用的mipmap的方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		//设置包裹方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//u
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//v
	}
	stbi_image_free(data);
}

Texture::~Texture()
{
	if (mTexture != 0) {
		glDeleteTextures(1, &mTexture);
	}
}


void Texture::bind()
{
	//先切换自家的纹理单元
	glActiveTexture(GL_TEXTURE0 + mUnit);
	//再绑定自己到现在的纹理状态机上去
	glBindTexture(GL_TEXTURE_2D, mTexture);
}