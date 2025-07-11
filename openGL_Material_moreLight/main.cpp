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
#include "glFrameWork/DirectionalLight.h"
#include "glFrameWork/PointLight.h"

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
glm::mat4 modelLight[3] = { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) };


glm::mat4 view(1.0);
glm::mat4 projection(1.0);
glm::mat4 transform(1.0);
glm::mat4 transformLight[3] = { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) };


int screenWidth = 800;
int screenHeight = 600;

PerspectiveCamera* camera = nullptr;
TrackBallCameraControl* cameraControl = nullptr;


PointLight* pointlight = new PointLight[3];
DirectionalLight* directionallight = new DirectionalLight();

glm::vec3 cubePosition[10] =
{
	{0.0f, 0.0f, 0.0f},
	{4.0f, 4.0f, 4.0f},
	{4.0f, -4.0f, -4.0f},
	{6.0f, 2.0f, -8.0f},
	{6.0f, -4.0f, 9.0f},
	{7.0f, -1.0f, 9.0f},
	{9.0f, 9.0f, -5.0f},
	{10.0f, -8.0f, -11.0f},
	{12.0f, 10.0f, -13.0f},
	{-9.0f, -15.0f, 1.0f}
};

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

	for( int i = 0 ; i < 3 ; i ++)
	{
		modelLight[i] = glm::translate(glm::mat4(1.0), pointlight[i].position);
	}
	
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
	for( int i = 0 ; i < 3 ; i ++)
		transformLight[i] = projection * view * modelLight[i];
}



void prepareVAOandLight()
{
	geometry = Geometry::createBox(3.0f);
	light = Geometry::createSphere(0.4f); //点灯的mesh

	//点灯的属性
	pointlight[0].ambient = vec3(0.2f, 0.2f, 0.2f);
	pointlight[0].diffuse = vec3(1.0f, 1.0f, 0.1f);
	pointlight[0].specular = vec3(1.0, 1.0f, 1.0f);
	pointlight[0].position = vec3 (0.7f, 3.0f, 0.9f);
	pointlight[0].constant = 1.0f;
	pointlight[0].linear = 0.09f;
	pointlight[0].quadratic = 0.032f;
	pointlight[1].ambient = vec3(0.2f, 0.2f, 0.2f);
	pointlight[1].diffuse = vec3(1.0f, 0.1f, 1.0f);
	pointlight[1].specular = vec3(1.0, 1.0f, 1.0f);
	pointlight[1].position = vec3(6.7f, 3.0f, 3.9f);
	pointlight[1].constant = 1.0f;
	pointlight[1].linear = 0.09f;
	pointlight[1].quadratic = 0.032f;
	pointlight[2].ambient = vec3(0.2f, 0.2f, 0.2f);
	pointlight[2].diffuse = vec3(0.1f, 0.9f, 0.4f);
	pointlight[2].specular = vec3(1.0, 1.0f, 1.0f);
	pointlight[2].position = vec3(3.7f, -3.0f, 4.9f);
	pointlight[2].constant = 1.0f;
	pointlight[2].linear = 0.09f;
	pointlight[2].quadratic = 0.032f;

	//平行光属性
	directionallight->ambient = vec3(0.2f, 0.2f, 0.2f);
	directionallight->diffuse = vec3(0.7f, 0.7f, 0.7f);
	directionallight->specular = vec3(1.0, 1.0f, 1.0f);
	directionallight->direction = vec3(1.0f, 0.0f, 0.0f);

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


	//启用正方体的shader
	shader->begin();

	//材质
	//shader->setUniformInt("sampler", 0);

	//shader->setUniformFloat("time", glfwGetTime());
	//shader->setUniformVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
	//shader->setUniformVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

	//灯光相机位置信息传着色器
	//shader->setUniformVec3("lightPos", lightPos);
	shader->setUniformVec3("viewPos", camera->mPosition);

	// 所有点光源的属性
	for (int i = 0; i < 3; i++)
	{
		std::string baseName = "pointLights[" + std::to_string(i) + "].";  // 动态拼接索引

		shader->setUniformVec3(baseName + "ambient", pointlight[i].ambient);
		shader->setUniformVec3(baseName + "diffuse", pointlight[i].diffuse);
		shader->setUniformVec3(baseName + "specular", pointlight[i].specular);
		shader->setUniformVec3(baseName + "position", pointlight[i].position);
		shader->setUniformFloat(baseName + "constant", pointlight[i].constant);
		shader->setUniformFloat(baseName + "linear", pointlight[i].linear);
		shader->setUniformFloat(baseName + "quadratic", pointlight[i].quadratic);
	}

	//平行光设置
	shader->setUniformVec3("dirLight.direction", directionallight->direction);
	shader->setUniformVec3("dirLight.ambient", directionallight->ambient);
	shader->setUniformVec3("dirLight.diffuse", directionallight->diffuse);
	shader->setUniformVec3("dirLight.specular", directionallight->specular);


	//绘制很多个正方体
	glBindVertexArray(geometry->getVao());
	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 each_model = glm::mat4(1.0f);  // 默认单位矩阵
		float temp_angle = 20.0f * i;
		each_model = glm::rotate(each_model, glm::radians(temp_angle), glm::vec3(1.0f, 0.3f, 0.5f)); // 先旋转
		each_model = glm::translate(each_model, cubePosition[i]);  // 后平移


		glm::mat4 each_transform = projection * view * each_model;
		//变换矩阵传给着色器
		shader->setUniformMatrix4x4("transform", each_transform);
		shader->setUniformMatrix4x4("model", each_model);

		//立方体的材质设置传着色器
		shader->setUniformInt("material.diffuse", 0);//其实是纹理单元作为diffuse
		shader->setUniformVec3("material.specular", vec3(0.5f, 0.5f, 0.5f));
		shader->setUniformFloat("material.shininess", 32.0f);

		glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);
		
	}
	glBindVertexArray(0);
	

	shader->end();


	//绘制light的外观，所以需要先打开light外观的shader绘制
	shaderLight->begin();
	glBindVertexArray(light->getVao());
	for (int i = 0; i < 3; i++)
	{
		shaderLight->setUniformMatrix4x4("transform", transformLight[i]);
		shaderLight->setUniformVec3("lookslikeColor", pointlight[i].diffuse);

		glDrawElements(GL_TRIANGLES, light->getIndicesCount(), GL_UNSIGNED_INT, 0);
		
	}
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

	prepareVAOandLight();
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
	delete geometry , directionallight ;
	delete[] pointlight;
	return 0;
}