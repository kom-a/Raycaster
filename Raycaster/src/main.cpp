#define _USE_MATH_DEFINES

#include <iostream>

#include "window.h"
#include "input/input.h"
#include "shader.h"
#include "renderer.h"
#include "map.h"
#include "player.h"
#include "spritesheet.h"
#include "sprite.h"

#include <glad/glad.h>

int main()
{
	uint32_t width = 640, height = 480;

	Window window(width, height, "Raycaster");
	Renderer renderer(width, height);

	Player player(6.01f, 3.1f, 0.0f);
	Camera camera((float)M_PI / 3.0f);

	SpriteSheet sheet("res/spritesheet.bmp", 64, 64);
	Map map("res/maps/textureTestMap.rcm");
	
	Sprite sprite("res/tronchungo3.png", 3, 4);
	Sprite sprite2("res/Warrior_Idle_1.png", 5, 2);

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

		player.Update(deltaTime);

		renderer.Clear();

		for (uint32_t x = 0; x < width; x++)
		{
			float angle = (player.GetAngle() - camera.GetFOV() / 2.0f) + (float)x / width * camera.GetFOV();

			char tex;
			glm::vec2 hit = camera.CastRay(player.GetPosition(), glm::vec2(glm::cos(angle), glm::sin(angle)), map, &tex);

			float hitX = hit.x - int(hit.x + 0.5f);
			float hitY = hit.y - int(hit.y + 0.5f);

			int texColumn;
			if (glm::abs(hitY) > glm::abs(hitX))
				texColumn = (int)(hitY * 64);
			else
				texColumn = (int)(hitX * 64);

			if (texColumn < 0)
			{
				texColumn += 64;
			}

			float distance = glm::length(hit - player.GetPosition());
			Texture texture = sheet[tex];

			renderer.DrawWall(x, distance, texture, texColumn, player);
		}

		renderer.DrawSprite(sprite, player);
		renderer.DrawSprite(sprite2, player);
		
		int error = glGetError();
		if (error)
			std::cout << "OpenGL ERROR: " << error << std::endl;

		renderer.Flush();

		window.Update();
		fps++;
	}

	return 0;
}