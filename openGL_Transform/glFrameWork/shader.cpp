#include "shader.h"
#include "../wrapper/checkError.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	//从文件路径中获取顶点 / 片段着色器
	std::string vertexCode, fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// 保证ifstream对象可以抛出异常：
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// 打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		//读取文件的缓冲内容到数据流stringstream中

		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		//将字符从流中取出来，存到string里面
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}


	//shader 创建
	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();

	unsigned int vertexShader, fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); //申请一个顶点着色器的编号
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //将着色器代码绑定到vertexShader编号还是上
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	//shader 编译
	glCompileShader(vertexShader); //编译这个编号的着色器
	checkShaderError(vertexShader, "COMPILE");
	glCompileShader(fragmentShader);
	checkShaderError(fragmentShader, "COMPILE");


	//shader 链接成着色器程序
	mProgram = glCreateProgram();

	glAttachShader(mProgram, vertexShader);//先附加
	glAttachShader(mProgram, fragmentShader);
	glLinkProgram(mProgram); //链接
	checkShaderError(mProgram, "LINK");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
Shader::~Shader()
{

}

void Shader::begin() //开始使用这个shader
{
	glUseProgram(mProgram);
}
void Shader::end() //结束使用这个shader
{
	glUseProgram(0);
}

void Shader::setUniformFloat(const std::string& name, float value)
{
	//通过你需要改的uniform变量的名字拿到uniform变量的位置
	GLint location = CK(glGetUniformLocation(mProgram, name.c_str()));
	//改成你需要的值

	CK(glUniform1f(location, value));
}

void Shader::setUniformInt(const std::string& name, int value)
{
	//通过你需要改的uniform变量的名字拿到uniform变量的位置
	GLint location = CK(glGetUniformLocation(mProgram, name.c_str()));
	//改成你需要的值
	CK(glUniform1i(location, value));
}

void Shader::setUniformMatrix4x4(const std::string& name, glm::mat4 value)
{
	GLint location = CK(glGetUniformLocation(mProgram, name.c_str()));

	//参数说明：uniform变量的位置 ， 几个变量 ，是否转置 ,矩阵对象内容的地址
	CK(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
}


void Shader::checkShaderError(GLuint target, std::string type) {
	int  success = 0;
	char infoLog[512];
	if (type == "COMPILE")
	{
		glGetShaderiv(target, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(target, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
	else if (type == "LINK")
	{
		glGetProgramiv(target , GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(target, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}
	else
	{
		std::cout << "不知道你传了什么类型 ， 其实可以用枚举的，算了 owo \n";
	}
}