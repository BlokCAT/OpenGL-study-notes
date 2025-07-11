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
	//��ʼ����������
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
	glfwMakeContextCurrent(mWindow); //���ú������ģ�Ȼ��ſ��Գ�ʼ��glad

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Application::Failed to initialize GLAD" << std::endl;
		return false;
	}


	/*���úü����Ļص����� �� ����Ӧ���������������͵��ú�����������*/
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
	glfwPollEvents(); //���ַܷ�������Ϣ��������̣���꣬Ȼ����������

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
	//���м�����Ӧ��ʱ�򣬻ص������ͻ��ȡ�������
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