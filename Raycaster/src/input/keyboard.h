#pragma once

#define MAX_KEYS 512

#include <GLFW/glfw3.h>

class Keyboard
{
public:
	inline static bool IsKeyPressed(int key) { return s_Keys[key]; }

private:
	static bool s_Keys[MAX_KEYS];

	friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);