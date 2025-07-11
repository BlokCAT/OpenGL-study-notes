#include "shader.h"
#include "../wrapper/checkError.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	//���ļ�·���л�ȡ���� / Ƭ����ɫ��
	std::string vertexCode, fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// ��֤ifstream��������׳��쳣��
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// ���ļ�
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		//��ȡ�ļ��Ļ������ݵ�������stringstream��

		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		//���ַ�������ȡ�������浽string����
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}


	//shader ����
	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();

	unsigned int vertexShader, fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); //����һ��������ɫ���ı��
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //����ɫ������󶨵�vertexShader��Ż�����
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	//shader ����
	glCompileShader(vertexShader); //���������ŵ���ɫ��
	checkShaderError(vertexShader, "COMPILE");
	glCompileShader(fragmentShader);
	checkShaderError(fragmentShader, "COMPILE");


	//shader ���ӳ���ɫ������
	mProgram = glCreateProgram();

	glAttachShader(mProgram, vertexShader);//�ȸ���
	glAttachShader(mProgram, fragmentShader);
	glLinkProgram(mProgram); //����
	checkShaderError(mProgram, "LINK");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
Shader::~Shader()
{

}

void Shader::begin() //��ʼʹ�����shader
{
	glUseProgram(mProgram);
}
void Shader::end() //����ʹ�����shader
{
	glUseProgram(0);
}

void Shader::setUniformFloat(const std::string& name, float value)
{
	//ͨ������Ҫ�ĵ�uniform�����������õ�uniform������λ��
	GLint location = CK(glGetUniformLocation(mProgram, name.c_str()));
	//�ĳ�����Ҫ��ֵ

	CK(glUniform1f(location, value));
}

void Shader::setUniformInt(const std::string& name, int value)
{
	//ͨ������Ҫ�ĵ�uniform�����������õ�uniform������λ��
	GLint location = CK(glGetUniformLocation(mProgram, name.c_str()));
	//�ĳ�����Ҫ��ֵ
	CK(glUniform1i(location, value));
}

void Shader::setUniformMatrix4x4(const std::string& name, glm::mat4 value)
{
	GLint location = CK(glGetUniformLocation(mProgram, name.c_str()));

	//����˵����uniform������λ�� �� �������� ���Ƿ�ת�� ,����������ݵĵ�ַ
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
		std::cout << "��֪���㴫��ʲô���� �� ��ʵ������ö�ٵģ����� owo \n";
	}
}