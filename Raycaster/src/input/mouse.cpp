#include "Mouse.h"

double Mouse::s_MouseX;
double Mouse::s_MouseY;
double Mouse::s_LastMouseX;
double Mouse::s_LastMouseY;
bool Mouse::s_Buttons[MAX_BUTTONS];

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Mouse::s_LastMouseX = Mouse::s_MouseX;
	Mouse::s_LastMouseY = Mouse::s_MouseY;
	Mouse::s_MouseX = xpos;
	Mouse::s_MouseY = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Mouse::s_Buttons[button] = action != GLFW_RELEASE;
}