#include<iostream>
#include"glFrameWork/core.h"
#include"glFrameWork/shader.h"
#include"glFrameWork/texture.h"
#include <string>
#include "wrapper/checkError.h"
#include "application/Application.h"
#
using namespace std;

float vertices[] = {
	0.0f, 1.0f, 0.0f,   
	-0.5f, -0.5f, 0.0f,  
	0.5f, -0.5f, 0.0f, 
};
float colors[] = {
	1.0f , 0.0f , 0.0f,
	0.0f , 1.0f , 0.0f,
	0.0f , 0.0f , 1.0f
};

float uvs[] = {
	0.5f , 1.0f,
	0.0f , 0.0f,
	1.0f , 0.0f
	
};

unsigned int indices[] = {
	0, 1, 2, // ��һ��������

};



GLuint vao, EBO;
Shader* shader = nullptr;
float ans = 0.0f;
Texture* tex = nullptr;





//���ڻص�����
void myWindowSizeCallback( int width, int height)
{
	//һЩ���ڱ仯֮�������Ӧ����
	CK(glViewport(0, 0, width, height));
	cout << width << " " << height << endl;
}

//���̻ص�����
void mykeyCallback( int key, int action, int mods)
{
	//���°���֮����Ҫ��������
	/*if (key == GLFW_KEY_W && action == GLFW_PRESS) 
	{
		std::cout << "�㰴����W" << std::endl;
		std::cout << "action" << action << std::endl;
		std::cout << "mods" << mods <<  std::endl;
		
		if(ans < 1.0f) ans += 0.1;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		std::cout << "�㰴����S" << std::endl;
		std::cout << "action" << action << std::endl;
		std::cout << "mods" << mods << std::endl;

		if (ans > 0.0f) ans -= 0.1;
	}*/
		
}


void prepareVAO()
{
	//VAO -> ��VBO -> ��EBO -> ���VAO

	//����VAO ����ṹ
	glGenVertexArrays(1, &vao);  //һ��vao �� Ȼ��ڶ����������Ƿ��ص�vao����

	//�����vao
	glBindVertexArray(vao); 

	//����VBO����VAO
	GLuint posvbo = 0, colorvbo = 0 , uvvbo;
	CK(glGenBuffers(1, &posvbo));
	CK(glBindBuffer(GL_ARRAY_BUFFER, posvbo));
	CK(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	CK(glGenBuffers(1, &colorvbo));
	CK(glBindBuffer(GL_ARRAY_BUFFER, colorvbo));
	CK(glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	CK(glGenBuffers(1, &uvvbo));
	CK(glBindBuffer(GL_ARRAY_BUFFER, uvvbo));
	CK(glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW));
	glEnableVertexAttribArray(2);
	 //�������ã���λ �� �������������м���������� �� ������ʲô���� �� ��Ҫ��һ���� ��ÿһ�����ԵĴ�С�������������float�Ĵ�С�����ڲ�ƫ�ƴ�С 
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);


	//����EBO����VAO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBindVertexArray(0); //���Ŀǰ���е�vao ����Ϊǰ��ȫ�����͵��Դ�����ȥ��

  

}

void prepareShader()
{
	shader = new Shader("assets/shader/vertex.glsl", "assets/shader/fragment.glsl");
}

void prepareTexture()
{
	tex = new Texture("assets/texture/w.jpg", 0);
}

void render() {//ִ��opebGL��Ⱦ���� 


	glClear(GL_COLOR_BUFFER_BIT);
	//���������ɫ����
	shader->begin();
	shader->setUniformInt("sampler", 0);
	shader->setUniformFloat("time", glfwGetTime());
	shader->setUniformFloat("w", tex->getWidth());
	shader->setUniformFloat("h", tex->getHeight());

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	shader->end();
}
int main()
{
	if (!app->init(800, 600, "openGL")) 
	{
		cout << "glfw��ʼ��ʧ��" <<endl;
		return 0;
	}
	app->setResizeCallback(myWindowSizeCallback);
	app->setKeyCallback(mykeyCallback);
	//�����Ļص�����ע��
	//glfwSetFramebufferSizeCallback(window, WindowSizeCallback);
	

	 //�����ӿ��Լ������� ɫ
	glViewport(0, 0, 800, 600);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	prepareVAO();
	prepareShader();
	prepareTexture();
	//ִ�д���ѭ��
	while (app->update())
	{
		
		render();

		//��Ⱦ����
	}
	
	app->destroy();
	delete shader;
	delete tex;

	return 0;
}