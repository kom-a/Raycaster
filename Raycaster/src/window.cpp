#include <iostream>

#include "window.h"

#include "input/mouse.h"
#include "input/keyboard.h"

Window* Window::INSTANCE;

Window::Window(int width, int height, const char* title)
	: m_Width(width), m_Height(height), m_Title(title)
{
	if(INSTANCE == nullptr)
		Init();
}

Window::~Window()
{
	glfwDestroyWindow(m_GLFWwindow);
	glfwTerminate();
}

void Window::Init()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW." << std::endl;
		return;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	m_GLFWwindow = glfwCreateWindow(m_Width, m_Height, m_Title, nullptr, nullptr);
	glfwMakeContextCurrent(m_GLFWwindow);
	glfwSetFramebufferSizeCallback(m_GLFWwindow, window_resize_callback);
	glfwSetCursorPosCallback(m_GLFWwindow, cursor_position_callback);
	glfwSetMouseButtonCallback(m_GLFWwindow, mouse_button_callback);
	glfwSetKeyCallback(m_GLFWwindow, key_callback);
	glfwSetInputMode(m_GLFWwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSwapInterval(0);

	glfwSetWindowUserPointer(m_GLFWwindow, this);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize glad" << std::endl;
		glfwTerminate();
		return;
	}
}

void Window::Close() const
{
	glfwSetWindowShouldClose(m_GLFWwindow, GLFW_TRUE);
}

bool Window::IsClosed() const
{
	return glfwWindowShouldClose(m_GLFWwindow);
}

void Window::DisableCursor(bool disable)
{
	if(disable)
		glfwSetInputMode(m_GLFWwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else 
		glfwSetInputMode(m_GLFWwindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void  Window::Update()
{
	glfwPollEvents();

	int error = glGetError();
	if (error != 0)
		std::cout << "OpenGL error: " << error << std::endl;

	glfwSwapBuffers(m_GLFWwindow);
}

void window_resize_callback(GLFWwindow* window, int width, int height)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->m_Width = width;
	win->m_Height = height;
	glViewport(0, 0, width, height);
}