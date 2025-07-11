#include<iostream>
#include"glFrameWork/core.h"
#include"glFrameWork/shader.h"
#include <string>
#include "wrapper/checkError.h"
#include "application/Application.h"

#define STB_IMAGE_IMPLEMENTATION
#include "application/stb_image.h" //��ȡͼƬ���ⲿ��
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



GLuint vao, EBO, TextureID, TextureID2;
Shader* shader = nullptr;
float ans = 0.0f;
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
	if (key == GLFW_KEY_W && action == GLFW_PRESS) 
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
	}
		
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
	int w, h, channels; 
	int w2, h2, channels2;

	stbi_set_flip_vertically_on_load(true); //��תy��

	//��ͼƬ
	unsigned char* data = stbi_load("assets/texture/w.jpg", &w, &h, &channels, STBI_rgb_alpha);
	unsigned char* data2 = stbi_load("assets/texture/q.png" , &w2 , &h2 , &channels2 , STBI_rgb_alpha);

	if (data && data2)
	{
		glGenTextures(1, &TextureID);//�����������
		glActiveTexture( GL_TEXTURE0);//����0������Ԫ
		glBindTexture(GL_TEXTURE_2D, TextureID); //������GL_TEXTURE_2D״̬����ͬʱ0������Ԫ��Ŀǰ���������ҹ���
		//��GPU������������,�Ὺ���Դ�
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//���ù��˷�ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//���ð�����ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//u
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//v


		glGenTextures(1, &TextureID2);//�����������
		glActiveTexture(GL_TEXTURE1);//����1������Ԫ
		glBindTexture(GL_TEXTURE_2D, TextureID2); //������GL_TEXTURE_2D״̬����ͬʱ1������Ԫ��Ŀǰ���������ҹ���
		//��GPU������������,�Ὺ���Դ�
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w2, h2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		//���ù��˷�ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//���ð�����ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//u
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//v
	
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	stbi_image_free(data2);

}

void render() {//ִ��opebGL��Ⱦ���� 


	glClear(GL_COLOR_BUFFER_BIT);
	//���������ɫ����
	shader->begin();

	
	shader->setUniformInt("sampler", 0);
	shader->setUniformInt("sampler2", 1);
	shader->setUniformFloat("cnt", ans);
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
	

	 //�����ӿ��Լ�������ɫ
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
	return 0;
}