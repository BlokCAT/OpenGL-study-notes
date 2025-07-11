#pragma once
#include "core.h"
#include <string>

class Shader 
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void begin(); //��ʼʹ�����shader
	void end(); //����ʹ�����shader

	void setUniformFloat(const std::string& name, float value);

	void setUniformInt(const std::string& name, int value);
private:
	GLuint mProgram = { 0 };
	void checkShaderError(GLuint target, std::string type);
};