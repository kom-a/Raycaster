#define _USE_MATH_DEFINES

#include <iostream>

#include "window.h"
#include "input/input.h"
#include "shader.h"
#include "renderer.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef struct
{
	uint8_t* data;
	int width, height, channels;
} texture_t;

texture_t* load_texture(const char* filename)
{
	stbi_set_flip_vertically_on_load(false);
	texture_t* texture = new texture_t;
	texture->data = stbi_load(filename, &texture->width, &texture->height, &texture->channels, 4);

	if (!texture->data)
	{
		std::cout << "Failed to load texture." << std::endl;
	}

	return texture;
}

int main()
{
	uint32_t width = 1280, height = 720;

	Window window(width, height, "Raycaster");
	Renderer renderer(width, height);

	double lastTime = glfwGetTime();
	double deltaTime = 0;
	double unprocessedTime = 0;
	int fps = 0;

	char map[] = {
		"0000000000000000"
		"0              0"
		"0              0"
		"0              0"
		"0              0"
		"0    1111      0"
		"0              0"
		"0              0"
		"0              0"
		"0              0"
		"0              0"
		"0      22322   0"
		"0              0"
		"0              0"
		"0              0"
		"0000000000000000"
	};

	size_t rectWidth = width / 16;
	size_t rectHeight = height / 16;

	float playerX = 6.01f;
	float playerY = 4.1f;
	float playerS = 2.5f; // PLayer Speed
	float playerA = 0; // PLayer angle
	const float fov = M_PI / 3; // Field of view

	texture_t* texture = load_texture("res/walltext.png");

	int lastMouseX = 0;
	bool firstMouse = true;

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


		float speed = playerS * deltaTime;
		if (Keyboard::IsKeyPressed(GLFW_KEY_ESCAPE))
			window.Close();

		glm::vec2 moveDirection(0);

		if (Keyboard::IsKeyPressed(GLFW_KEY_W))
		{
			moveDirection += glm::vec2(glm::cos(playerA), glm::sin(playerA));
		}
		if (Keyboard::IsKeyPressed(GLFW_KEY_S))
		{
			moveDirection -= glm::vec2(glm::cos(playerA), glm::sin(playerA));
		}
		if (Keyboard::IsKeyPressed(GLFW_KEY_A))
		{
			float alpha = playerA - M_PI / 2;
			moveDirection += glm::vec2(glm::cos(alpha), glm::sin(alpha));
		}
		if (Keyboard::IsKeyPressed(GLFW_KEY_D))
		{
			float alpha = playerA + M_PI / 2;
			moveDirection += glm::vec2(glm::cos(alpha), glm::sin(alpha));
		}

		if (moveDirection != glm::vec2(0))
			moveDirection = glm::normalize(moveDirection);
		playerX += moveDirection.x * speed;
		playerY += moveDirection.y * speed;

		if (firstMouse && false)
		{
			lastMouseX = Mouse::GetX();
			firstMouse = false;
		}

		playerA += (Mouse::GetX() - lastMouseX) * 0.0025f;
		lastMouseX = Mouse::GetX();

		renderer.Clear();

		for (size_t x = 0; x < width; x++)
		{
			float angle = (playerA - fov / 2.0f) + (float)x / width * fov;
			glm::vec2 rayStart(playerX, playerY);
			glm::vec2 rayDir(glm::cos(angle), glm::sin(angle));
			glm::vec2 rayUnitStepSize(glm::sqrt(1 + (rayDir.y / rayDir.x) * (rayDir.y / rayDir.x)), glm::sqrt(1 + (rayDir.x / rayDir.y) * (rayDir.x / rayDir.y)));
			
			glm::ivec2 mapCheck(rayStart.x, rayStart.y);
			glm::vec2 rayLength1D;

			glm::ivec2 step;

			if (rayDir.x < 0)
			{
				step.x = -1;
				rayLength1D.x = (rayStart.x - float(mapCheck.x)) * rayUnitStepSize.x;
			}
			else
			{
				step.x = 1;
				rayLength1D.x = (float(mapCheck.x + 1) - rayStart.x) * rayUnitStepSize.x;
			}

			if (rayDir.y < 0)
			{
				step.y = -1;
				rayLength1D.y = (rayStart.y - float(mapCheck.y)) * rayUnitStepSize.y;
			}
			else
			{
				step.y = 1;
				rayLength1D.y = (float(mapCheck.y + 1) - rayStart.y) * rayUnitStepSize.y;
			}

			bool found = false;
			float maxDistance = 30.0f;
			float distance = 0.0f;
			while (!found && distance < maxDistance)
			{
				if (rayLength1D.x < rayLength1D.y)
				{
					mapCheck.x += step.x;
					distance = rayLength1D.x;
					rayLength1D.x += rayUnitStepSize.x;
				}
				else
				{
					mapCheck.y += step.y;
					distance = rayLength1D.y;
					rayLength1D.y += rayUnitStepSize.y;
				}
				
				if (map[mapCheck.y * 16 + mapCheck.x] != ' ')
				{
					glm::ivec3 color;
					size_t tex;
					color = glm::ivec3(100 * (1 - distance / 25.0), 200 * (1 - distance / 25.0), 50 * (1 - distance / 25.0));

					float cx = playerX + distance * glm::cos(angle);
					float cy = playerY + distance * glm::sin(angle);

					float hitX = cx - int(cx + 0.5f);
					float hitY = cy - int(cy + 0.5f);

					int texColumn;
					if(glm::abs(hitY) > glm::abs(hitX))
						texColumn = hitY * 64;
					else
						texColumn = hitX * 64;

					if (texColumn < 0)
					{
						texColumn += 64;
					}

					switch (map[mapCheck.y * 16 + mapCheck.x] - '0')
					{
						case 0: tex = 0; break;
						case 1: tex = 1; break;
						case 2: tex = 2; break;
						case 3: tex = 3; break;
						case 4: tex = 4; break;
						case 5: tex = 5; break;
						default: std::cout << "Incorrect texture index\n"; break;
					}

					int wallHeight = height / (distance * cos(angle - playerA));
					int top = height / 2 - wallHeight / 2;
					for (size_t y = 0; y < wallHeight; y++)
					{
						int texY = (float)y / wallHeight * texture->height;
						int texX = texColumn + 64 * tex;

						int r = (texture->data[(texY * texture->width + texX) * 4 + 0]);
						int g = (texture->data[(texY * texture->width + texX) * 4 + 1]);
						int b = (texture->data[(texY * texture->width + texX) * 4 + 2]);
						// int a = (texture->data[(y * texture->width + tex * 64 + x) * 4 + 3]);
						renderer.Draw(x, top + y, glm::ivec3(r, g, b));
					}

					/*switch (map[mapCheck.y * 16 + mapCheck.x] - '0')
					{
						case 0:
						{
							int r = (texture->data[(y * texture->width + tex * 64 + x) * 4 + 0]);
							int g = (texture->data[(y * texture->width + tex * 64 + x) * 4 + 1]);
							int b = (texture->data[(y * texture->width + tex * 64 + x) * 4 + 2]);
							int a = (texture->data[(y * texture->width + tex * 64 + x) * 4 + 3]);
						}break;
					}
					int r = (texture->data[(y * texture->width + tex * 64 + x) * 4 + 0]);
					int g = (texture->data[(y * texture->width + tex * 64 + x) * 4 + 1]);
					int b = (texture->data[(y * texture->width + tex * 64 + x) * 4 + 2]);
					int a = (texture->data[(y * texture->width + tex * 64 + x) * 4 + 3]);*/
					
					//renderer.DrawWall(x, height / (distance * cos(angle - playerA)), color);
					found = true;
				}
			}
		}

		/*for (int y = 0; y < 64; y++)
		{
			for (int x = 0; x < 64; x++)
			{
				int r = (texture->data[(y * texture->width + tex * 64 + x) * 4 + 0]);
				int g = (texture->data[(y * texture->width + tex * 64 + x) * 4 + 1]);
				int b = (texture->data[(y * texture->width + tex * 64 + x) * 4 + 2]);
				int a = (texture->data[(y * texture->width + tex * 64 + x) * 4 + 3]);
				renderer.Draw(x, y, glm::ivec3(r, g, b));
			}
		}*/

		for (size_t y = 0; y < 64; y++)
		{
			for (size_t x = 0; x < 64; x++)
			{
				size_t mapY = (y / 64.0f * 16);
				size_t mapX = (x / 64.0f * 16);
				if (map[mapY * 16 + mapX] == ' ')
					renderer.Draw(x, y, glm::ivec3(255));
				else
					renderer.Draw(x, y, glm::ivec3(50, 200, 100));
			}
		}
		
		int error = glGetError();
		if (error)
			std::cout << "OpenGL ERROR: " << error << std::endl;

		renderer.Flush();

		window.Update();
		fps++;
	}

	return 0;
}