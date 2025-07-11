#pragma once
#include "core.h"
#include <string>

class Shader 
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void begin(); //开始使用这个shader
	void end(); //结束使用这个shader

	void setUniformFloat(const std::string& name, float value);

	void setUniformInt(const std::string& name, int value);

	void setUniformVec3(const std::string& name, glm::vec3 value);

	void setUniformMatrix4x4(const std::string& name, glm::mat4 value);
private:
	GLuint mProgram = { 0 };
	void checkShaderError(GLuint target, std::string type);
};