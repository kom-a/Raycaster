#include "Keyboard.h"


bool Keyboard::s_Keys[MAX_KEYS];

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Keyboard::s_Keys[key] = action != GLFW_RELEASE;
}