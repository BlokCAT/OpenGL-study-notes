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
#include "glFrameWork/geometry.h"

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



Geometry* geometry = nullptr;
Geometry* light = nullptr;

Shader* shader = nullptr;
Shader* shaderLight = nullptr;

Texture* tex = nullptr;

glm::mat4 model(1.0);
glm::mat4 modelLight(1.0);

glm::mat4 view(1.0);
glm::mat4 projection(1.0);
glm::mat4 transform(1.0);
glm::mat4 transformLight(1.0);


int screenWidth = 800;
int screenHeight = 600;

PerspectiveCamera* camera = nullptr;
TrackBallCameraControl* cameraControl = nullptr;

glm::vec3 lightPos(0.7f, 3.0f, 0.9f);  //灯的位置



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
	//model = glm::rotate(glm::mat4(1.0f), glm::radians(100.0f * (float)glfwGetTime()), glm::vec3(1.0, 1.0, 0.0));
	model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0, 0.0, 0.0));





	//translate是一个构造平移矩阵的函数，第一个参数是基于变换后的世界坐标，第二个是xyz方向上的平移的距离
	//transform = glm::translate(glm::mat4(1.0), glm::vec3(0.5f , 0.0f , 0.0f));
	//scale是一个构造缩放矩阵的函数，第一个参数是基于变换后的世界坐标，第二个是xyz方向上的缩放的距离
	//transform = glm::scale(glm::mat4(1.0), glm::vec3(0.1f, 0.1f, 1.0f));
	modelLight = glm::translate(glm::mat4(1.0), lightPos);
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
	transformLight = projection * view * modelLight;
}



void prepareVAO()
{
	geometry = Geometry::createBox(3.0f);
	light = Geometry::createSphere(0.4f);

}

void prepareShader()
{
	shader = new Shader("assets/shader/vertex.glsl", "assets/shader/fragment.glsl");
	shaderLight = new Shader("assets/shader/vertex.glsl", "assets/shader/lightshader.glsl");
}

void prepareTexture()
{
	tex = new Texture("assets/texture/w.png", 0);
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
	Transform();//对所有物体MVP变换


	//绘制正方体
	shader->begin();
	shader->setUniformInt("sampler", 0);
	shader->setUniformFloat("time", glfwGetTime());
	shader->setUniformVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
	shader->setUniformVec3("lightPos", lightPos);
	shader->setUniformVec3("viewPos", camera->mPosition);
	shader->setUniformVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	shader->setUniformMatrix4x4("transform", transform);
	shader->setUniformMatrix4x4("model", model);

	glBindVertexArray(geometry->getVao());
	glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	shader->end();

	//绘制light，所以需要先打开light的shader绘制
	shaderLight->begin();
	shaderLight->setUniformMatrix4x4("transform", transformLight);
	glBindVertexArray(light->getVao());
	glDrawElements(GL_TRIANGLES, light->getIndicesCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	shaderLight->end();


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
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	

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
	delete camera;
	delete cameraControl;
	delete geometry;
	return 0;
}