#include<iostream>
#include"glFrameWork/core.h"
#include"glFrameWork/shader.h"
#include <string>
#include "wrapper/checkError.h"
#include "application/Application.h"

#define STB_IMAGE_IMPLEMENTATION
#include "application/stb_image.h" //读取图片的外部库
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
	0, 1, 2, // 第一个三角形

};



GLuint vao, EBO, TextureID, TextureID2;
Shader* shader = nullptr;
float ans = 0.0f;
//窗口回调函数
void myWindowSizeCallback( int width, int height)
{
	//一些窗口变化之后，你的响应代码
	CK(glViewport(0, 0, width, height));
	cout << width << " " << height << endl;
}

//键盘回调函数
void mykeyCallback( int key, int action, int mods)
{
	//按下按键之后需要做的事情
	if (key == GLFW_KEY_W && action == GLFW_PRESS) 
	{
		std::cout << "你按下了W" << std::endl;
		std::cout << "action" << action << std::endl;
		std::cout << "mods" << mods <<  std::endl;
		
		if(ans < 1.0f) ans += 0.1;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		std::cout << "你按下了S" << std::endl;
		std::cout << "action" << action << std::endl;
		std::cout << "mods" << mods << std::endl;

		if (ans > 0.0f) ans -= 0.1;
	}
		
}


void prepareVAO()
{
	//VAO -> 加VBO -> 绑EBO -> 解绑VAO

	//构建VAO 这个结构
	glGenVertexArrays(1, &vao);  //一个vao ， 然后第二个参数就是返回的vao数组

	//绑定这个vao
	glBindVertexArray(vao); 

	//创建VBO加入VAO
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
	 //参数设置：槽位 ， 顶点的这个属性有几个数字组成 ， 数字是什么类型 ， 需要归一化吗 ，每一个属性的大小（这里就是两个float的大小），内部偏移大小 
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);


	//创建EBO加入VAO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBindVertexArray(0); //解绑目前所有的vao ，因为前面全部推送到显存里面去了

  

}

void prepareShader()
{
	shader = new Shader("assets/shader/vertex.glsl", "assets/shader/fragment.glsl");
}

void prepareTexture()
{
	int w, h, channels; 
	int w2, h2, channels2;

	stbi_set_flip_vertically_on_load(true); //翻转y轴

	//读图片
	unsigned char* data = stbi_load("assets/texture/w.jpg", &w, &h, &channels, STBI_rgb_alpha);
	unsigned char* data2 = stbi_load("assets/texture/q.png" , &w2 , &h2 , &channels2 , STBI_rgb_alpha);

	if (data && data2)
	{
		glGenTextures(1, &TextureID);//生成纹理对象
		glActiveTexture( GL_TEXTURE0);//激活0号纹理单元
		glBindTexture(GL_TEXTURE_2D, TextureID); //绑定纹理到GL_TEXTURE_2D状态机，同时0号纹理单元和目前的纹理对象挂钩了
		//向GPU传递纹理数据,会开辟显存
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//设置过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//设置包裹方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//u
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//v


		glGenTextures(1, &TextureID2);//生成纹理对象
		glActiveTexture(GL_TEXTURE1);//激活1号纹理单元
		glBindTexture(GL_TEXTURE_2D, TextureID2); //绑定纹理到GL_TEXTURE_2D状态机，同时1号纹理单元和目前的纹理对象挂钩了
		//向GPU传递纹理数据,会开辟显存
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w2, h2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		//设置过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//设置包裹方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//u
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//v
	
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	stbi_image_free(data2);

}

void render() {//执行opebGL渲染操作 


	glClear(GL_COLOR_BUFFER_BIT);
	//激活这个着色程序
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
		cout << "glfw初始化失败" <<endl;
		return 0;
	}
	app->setResizeCallback(myWindowSizeCallback);
	app->setKeyCallback(mykeyCallback);
	//监听的回调函数注册
	//glfwSetFramebufferSizeCallback(window, WindowSizeCallback);
	

	 //设置视口以及清理颜色
	glViewport(0, 0, 800, 600);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	prepareVAO();
	prepareShader();
	prepareTexture();
	//执行窗体循环
	while (app->update())
	{
		
		render();

		//渲染操作
	}
	
	app->destroy();
	delete shader;
	return 0;
}