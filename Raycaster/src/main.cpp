#include <iostream>

#include "window.h"
#include "input/input.h"
#include "shader.h"
#include "renderer.h"

#include <glad/glad.h>

int main()
{
	uint32_t width = 640, height = 480;

	Window window(width, height, "Raycaster");
	Renderer renderer(width, height);

	double lastTime = glfwGetTime();
	double deltaTime = 0;
	double unprocessedTime = 0;
	int fps = 0;

	while (!window.IsClosed())
	{
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		unprocessedTime += deltaTime;
		lastTime = currentTime;

		if (unprocessedTime >= 1.0)
		{
			std::cout << "FPS: " << fps << std::endl;
			fps = 0;
			unprocessedTime = 0;
		}

		glClear(GL_COLOR_BUFFER_BIT);

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if(i == j)
					renderer.Draw(j, i, glm::ivec3(0));
				else
					renderer.Draw(j, i, glm::ivec3(255, 100, 120));
			}
		}

		if(Keyboard::IsKeyPressed(GLFW_KEY_ESCAPE))
			window.Close();

		int error = glGetError();
		if (error)
			std::cout << "OpenGL ERROR: " << error << std::endl;

		renderer.Flush();

		window.Update();
		fps++;
	}

	return 0;
}