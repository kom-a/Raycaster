#pragma once

#define MAX_BUTTONS 16

#include <GLFW/glfw3.h>

class Mouse
{
public:
	inline static double GetX() { return s_MouseX; }
	inline static double GetY() { return s_MouseY; }
	inline static double GetLastX() { return s_LastMouseX; }
	inline static double GetLastY() { return s_LastMouseY; }
	inline static bool IsButtonPressed(int button) { return s_Buttons[button]; }

private:
	static bool s_Buttons[MAX_BUTTONS];
	static double s_MouseX, s_MouseY;
	static double s_LastMouseX, s_LastMouseY;

	friend void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);