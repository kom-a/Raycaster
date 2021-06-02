#include "Mouse.h"

double Mouse::s_MouseX;
double Mouse::s_MouseY;
bool Mouse::s_Buttons[MAX_BUTTONS];

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Mouse::s_MouseX = xpos;
	Mouse::s_MouseY = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Mouse::s_Buttons[button] = action != GLFW_RELEASE;
}