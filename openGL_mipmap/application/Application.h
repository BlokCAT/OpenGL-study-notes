#pragma once
#include<iostream>


#define app Application::getInstance() //全局唯一实例

class GLFWwindow;

using ResizeCallbackFunc = void(*)(int w, int h);//声明一个函数指针
using KeyCallbackFunc = void(*)( int key, int action, int mods);//声明一个函数指
using MouseMoveCallbackFunc = void(*)(double xpos, double ypos);
using MouseButtonCallbackFunc = void(*)(int button, int action, int mods);
using MouseScrollCallbackFunc = void(*)(double xoffset, double yoffset);

class Application {
public:
	~Application();

	static Application* getInstance(); //取出唯一的类对象的函数,由于是static所以直接通过类名可以调用
	uint32_t getWidth() const { return this->mWidth; }
	uint32_t getHeight()const { return this->mHeight; }


	bool init(const int& w , const int & h , const char *mname);
	bool update();
	void destroy();

	void setResizeCallback(ResizeCallbackFunc callbackFunc) {
		mResizeCallback = callbackFunc;
	}
	void setKeyCallback(KeyCallbackFunc callbackFunc) {
		mKeyCallback = callbackFunc;
	}
	void setMouseCallBack(MouseMoveCallbackFunc funcmove, MouseButtonCallbackFunc funcbutton, MouseScrollCallbackFunc funcscroll) {
		mMouseMoveCallback = funcmove;
		mMouseButtonCallback = funcbutton;
		mMouseScrollCallback = funcscroll;
	}
private:
	static void WindowSizeCallback(GLFWwindow * window , int w , int h );
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
private:
	Application(); 

	static Application* mInstance;

	uint32_t mWidth{ 0 };
	uint32_t mHeight{ 0 };
	GLFWwindow* mWindow{ nullptr };

	ResizeCallbackFunc mResizeCallback{nullptr};
	KeyCallbackFunc mKeyCallback{ nullptr };
	MouseMoveCallbackFunc mMouseMoveCallback{ nullptr };
	MouseButtonCallbackFunc mMouseButtonCallback{ nullptr };
	MouseScrollCallbackFunc mMouseScrollCallback{ nullptr };
};
 