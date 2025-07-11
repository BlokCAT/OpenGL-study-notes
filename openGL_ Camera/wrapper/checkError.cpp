#include"checkError.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string>
#include<iostream>
#include<assert.h>
using namespace std;
void checkError()
{
	GLenum errorcode = glGetError();
	string error = "";
	if (errorcode != GL_NO_ERROR)
	{
		switch (errorcode)
		{
		case GL_INVALID_ENUM:error = "GL_INVALID_ENUM"; break;
		case GL_INVALID_VALUE:error = "GL_INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:error = "GL_INVALID_OPERATION_my"; break;
		case GL_OUT_OF_MEMORY:error = "GL_OUT_OF_MEMORY"; break;
		default:
			error = "UNKONWN";
			break;
		}
		cout << error << endl;
		assert(false); //停止程序 ，显示调用堆栈
	}

}