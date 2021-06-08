#define _USE_MATH_DEFINES

#include <iostream>

#include "window.h"
#include "input/input.h"
#include "shader.h"
#include "renderer.h"
#include "map.h"
#include "player.h"
#include "spritesheet.h"

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

	const float fov = M_PI / 3.0f; // Field of view

	Player player(6.01f, 3.1f, 0.0f);

	SpriteSheet sheet("res/spritesheet.bmp", 64, 64);
	Map map("res/maps/textureTestMap.rcm");
	

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

		player.Update(deltaTime);

		if (Keyboard::IsKeyPressed(GLFW_KEY_ESCAPE))
			window.Close();

		renderer.Clear();

		for (size_t x = 0; x < width; x++)
		{
			float angle = (player.m_Angle - fov / 2.0f) + (float)x / width * fov;
			glm::vec2 rayStart(player.m_Position.x, player.m_Position.y);
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
			float maxDistance = 60.0f;
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
				
				if (map[mapCheck.y * map.GetWidth() + mapCheck.x] != ' ')
				{
					glm::ivec3 color;
					size_t tex;
					color = glm::ivec3(100 * (1 - distance / 25.0), 200 * (1 - distance / 25.0), 50 * (1 - distance / 25.0));

					float cx = player.m_Position.x + distance * glm::cos(angle);
					float cy = player.m_Position.y + distance * glm::sin(angle);

					float hitX = cx - int(cx + 0.5f);
					float hitY = cy - int(cy + 0.5f);

					int texColumn;
					if(glm::abs(hitY) > glm::abs(hitX))
						texColumn = (int)(hitY * 64);
					else
						texColumn = (int)(hitX * 64);

					if (texColumn < 0)
					{
						texColumn += 64;
					}

					switch (map[mapCheck.y * map.GetWidth() + mapCheck.x] - '0')
					{
						case 0: tex = 0; break;
						case 1: tex = 1; break;
						case 2: tex = 2; break;
						case 3: tex = 3; break;
						case 4: tex = 4; break;
						case 5: tex = 5; break;
						default: std::cout << "Incorrect texture index\n"; break;
					}

					uint32_t wallHeight = (uint32_t)(height / (distance * cos(angle - player.m_Angle)));
					int top = height / 2 - wallHeight / 2;
					
					Texture texture = sheet[tex];
					const uint8_t* column = texture.GetColumn(texColumn);

					for (size_t y = 0; y < wallHeight; y++)
					{
						/*int texY = (int)((float)y / wallHeight * texture->height);
						int texX = texColumn + 64 * tex;

						int r = (texture->data[(texY * texture->width + texX) * 4 + 0]) * (1 - distance / maxDistance);
						int g = (texture->data[(texY * texture->width + texX) * 4 + 1]) * (1 - distance / maxDistance);
						int b = (texture->data[(texY * texture->width + texX) * 4 + 2]) * (1 - distance / maxDistance);*/
						size_t mappedIndex = ((int)((float)y / wallHeight * texture.height)) * 4;
						int r = column[mappedIndex + 0];
						int g = column[mappedIndex + 1];
						int b = column[mappedIndex + 2];

						renderer.Draw(x, top + y + player.GetYOffset(), glm::ivec3(r, g, b));
					}

					found = true;
				}
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