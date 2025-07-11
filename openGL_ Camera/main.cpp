#include<iostream>
#include"glFrameWork/core.h"
#include"glFrameWork/shader.h"
#include"glFrameWork/texture.h"
#include <string>
#include "wrapper/checkError.h"
#include "application/Application.h"
#include "application/Camera/perspectiveCamera.h"
#include "application/camera/GameCameraControl.h"
#include "application/camera/trackBallCameraControl.h"



using namespace std;
using namespace glm;



float vertices[] = {
	-0.5f, 0.5f, 0.0f,   
	-0.5f, -0.5f, 0.0f,  
	0.5f, 0.5f, 0.0f, 
	0.5f,-0.5f,0.0f,

};
float colors[] = {
	1.0f , 0.0f , 0.0f,
	0.0f , 1.0f , 0.0f,
	0.0f , 0.0f , 1.0f,
	0.0f , 0.0f , 1.0f,

};

float uvs[] = {
	0.0f , 1.0f,
	0.0f , 0.0f,
	1.0f , 1.0f,
	1.0f , 0.0f
	
};

unsigned int indices[] = {
	0, 1, 2,
	2 ,1 ,3 ,
	4 , 5 , 6
};


GLuint vao, EBO;
Shader* shader = nullptr;
float ans = 0.0f;
Texture* tex = nullptr;

glm::mat4 model(1.0);
glm::mat4 view(1.0);
glm::mat4 projection(1.0);
glm::mat4 transform(1.0);


int screenWidth = 800;
int screenHeight = 600;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


PerspectiveCamera* camera = nullptr;
TrackBallCameraControl* cameraControl = nullptr;

void OnResize(int width, int height) {
	CK(glViewport(0, 0, width, height));
	std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
	cameraControl->onKey(key, action, mods);
}

//鼠标按下/抬起
void OnMouse(int button, int action, int mods) {
	double x, y;
	app->getCursorPosition(&x, &y);
	cameraControl->onMouse(button, action, x, y);
}

//鼠标移动
void OnCursor(double xpos, double ypos) {
	cameraControl->onCursor(xpos, ypos);
}

//鼠标滚轮
void OnScroll(double offset) {
	cameraControl->onScroll(offset);
}

void doModelTransform()
{
	//rotate是一个构造旋转矩阵的函数，第一个参数是基于的上次的变换（基于变换后的世界坐标），第二个是旋转的角度的弧度，第三个就是旋转轴
	model = glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
	//translate是一个构造平移矩阵的函数，第一个参数是基于变换后的世界坐标，第二个是xyz方向上的平移的距离
	//transform = glm::translate(glm::mat4(1.0), glm::vec3(0.5f , 0.0f , 0.0f));
	//scale是一个构造缩放矩阵的函数，第一个参数是基于变换后的世界坐标，第二个是xyz方向上的缩放的距离
	//transform = glm::scale(glm::mat4(1.0), glm::vec3(0.1f, 0.1f, 1.0f));
}
void doViewTransform()
{  
	//视图矩阵就是变换到相机空间，使用的就是lookat矩阵
	view = camera->getViewMatrix();
}

void doProjectionTransform()
{
	projection = camera->getProjectionMatrix();
}

void Transform()
{
	transform = projection * view * model;
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
	//参数设置：槽位 ， 顶点的这个属性有几个数字组成 ， 数字是什么类型 ， 需要归一化吗 ，每一个顶点的大小（ ​​从一个顶点到下一个顶点的字节跨度），内部偏移大小 
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
	 //参数设置：槽位 ， 顶点的这个属性有几个数字组成 ， 数字是什么类型 ， 需要归一化吗 ，每一个顶点的大小（ ​​从一个顶点到下一个顶点的字节跨度），内部偏移大小 
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
	tex = new Texture("assets/texture/w.jpg", 0);
}

void prepareCamera()
{
	float size = 6.0f;
	//camera = new OrthographicCamera(-size, size, size, -size, size, -size);
	camera = new PerspectiveCamera(
		60.0f,
		(float)app->getWidth() / (float)app->getHeight(),
		0.1f,
		1000.0f
	);

	cameraControl = new TrackBallCameraControl();
	cameraControl->setCamera(camera);
	cameraControl->setSensitivity(0.2f);
}

void render() {//执行opebGL渲染操作 


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	doModelTransform();
	doViewTransform();
	doProjectionTransform();
	Transform();//合并三个变换



	//激活这个着色程序
	shader->begin();
	shader->setUniformInt("sampler", 0);
	shader->setUniformFloat("time", glfwGetTime());
	shader->setUniformMatrix4x4("transform", transform);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	shader->end();
}
int main()
{
	if (!app->init(800, 600)) 
	{
		cout << "glfw初始化失败" <<endl;
		return 0;
	}

	 //设置视口以及清理颜色深度缓存
	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	

	app->setResizeCallback(OnResize);
	app->setKeyBoardCallback(OnKey);
	app->setMouseCallback(OnMouse);
	app->setCursorCallback(OnCursor);
	app->setScrollCallback(OnScroll);

	prepareVAO();
	prepareShader();
	prepareTexture();
	prepareCamera();
	
	glEnable(GL_DEPTH_TEST);
	//执行窗体循环
	while (app->update())
	{
		cameraControl->update();
		render();

		//渲染操作
	}
	
	app->destroy();
	delete shader;
	delete tex;

	return 0;
}