#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	static Window* INSTANCE;
public:
	Window(int width, int height, const char* title);
	~Window();

	void Update();
	void Close() const;
	bool IsClosed() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

	inline static Window* GetWindow() { return INSTANCE; }
	
private:
	void Init();

	friend void window_resize_callback(GLFWwindow* window, int width, int height);

private:
	int m_Width, m_Height;
	const char* m_Title;
	GLFWwindow* m_GLFWwindow;
};

void window_resize_callback(GLFWwindow* window, int width, int height);