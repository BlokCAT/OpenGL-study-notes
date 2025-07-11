#include "Application.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
Application* Application::mInstance = nullptr;
Application* Application::getInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new Application();
	}
	return mInstance;
}

Application::Application(){}
Application::~Application() {}


bool Application::init(const int& w, const int& h , const  char * window_name)
{
	mWidth = w;
	mHeight = h;
	//初始化基本环境
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mWindow = glfwCreateWindow(mWidth, mHeight, window_name, NULL, NULL);
	if (mWindow == NULL)
	{
		std::cout << "Application::Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(mWindow); //设置好上下文，然后才可以初始化glad

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Application::Failed to initialize GLAD" << std::endl;
		return false;
	}


	/*设置好监听的回调函数 ， 当对应的区域发生交互，就调用后面的这个函数*/
	glfwSetFramebufferSizeCallback(mWindow, WindowSizeCallback);
	glfwSetKeyCallback(mWindow, KeyCallback);



	return true;
}

bool Application::update()
{
	if (glfwWindowShouldClose(mWindow))
	{
		return false;
	}
	glfwPollEvents(); //接受分发窗口消息，比如键盘，鼠标，然后批量处理

	glfwSwapBuffers(mWindow);
	return true;
}

void Application::destroy() 
{
	glfwTerminate();
}

void Application::WindowSizeCallback(GLFWwindow* window, int w, int h) {
	if (Application::getInstance()->mResizeCallback != nullptr)
	{
		Application::getInstance()->mResizeCallback(w, h);
	}
}

void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//当有键盘响应的时候，回调函数就会调取这个函数
	if (Application::getInstance()->mKeyCallback != nullptr)
	{
		Application::getInstance()->mKeyCallback( key , action , mods);
	}
}

void Application::MouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
	if (Application::getInstance()->mMouseMoveCallback != nullptr)
	{
		Application::getInstance()->mMouseMoveCallback(xpos , ypos);
	}
}
void Application::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (Application::getInstance()->mMouseButtonCallback != nullptr)
	{
		Application::getInstance()->mMouseButtonCallback(button, action, mods);
	}
}
void Application::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	if (Application::getInstance()->mMouseScrollCallback != nullptr)
	{
		Application::getInstance()->mMouseScrollCallback(xoffset, yoffset);
	}
}