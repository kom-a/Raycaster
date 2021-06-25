#define _USE_MATH_DEFINES

#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <ctime>

#include "window.h"
#include "input/input.h"
#include "shader.h"
#include "renderer.h"
#include "map.h"
#include "player.h"
#include "spritesheet.h"
#include "sprite.h"
#include "utils/filemanager.h"
#include "utils/resourcemanager.h"

#include "enemies/goblin.h"
#include "enemies/flyingeye.h"

#include <glad/glad.h>

int main()
{
	uint32_t width = 640, height = width * 9 / 16;

	Window window(width, height, "Raycaster");
	Renderer renderer(width, height);

	Player player(7.1f, 3.0f, 3.1415f, "res/2.png", 250, 250);
	const Camera* camera = player.GetCamera();;

	SpriteSheet sheet("res/spritesheet.bmp", 64, 64, false);
	// const Texture* sky = FileManager::LoadTexture("res/Nebula Blue.png");
	Map map("res/maps/textureTestMap.rcm");

	ResourceManager::LoadSpriteSheet("res/Goblin/Idle.png", 33, 36, true, "GoblinIdleSheet");
	ResourceManager::LoadSpriteSheet("res/Goblin/Run.png", 36, 36, true, "GoblinRunSheet");
	ResourceManager::LoadSpriteSheet("res/Goblin/Attack2.png", 50, 48, false, "GoblinAttackSheet");
	ResourceManager::LoadSpriteSheet("res/Goblin/TakeHit.png", 42, 37, false, "GoblinTakeHitSheet");
	ResourceManager::LoadSpriteSheet("res/Goblin/Death.png", 58, 40, false, "GoblinDeathSheet");

	std::vector<Enemy*> enemies;
	for (int i = 0; i < 1; i++)
			enemies.push_back(new Goblin(glm::vec2(3.0 * i + 2, 3)));

	double lastTime = glfwGetTime();
	double deltaTime = 0;
	double unprocessedTime = 0;
	int fps = 0;

	srand((uint32_t) time(NULL));

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

		player.Update(deltaTime, map, enemies);
		for (Enemy* e : enemies)
		{
			e->Update(deltaTime, player);
		}

		std::sort(enemies.begin(), enemies.end(),
			[&](const Enemy* a, const Enemy* b) -> bool
			{
				// TODO: Optimize this

				float len1 = glm::length(player.GetPosition() - a->GetPosition());
				float len2 = glm::length(player.GetPosition() - b->GetPosition());
				return len1 < len2;
			});

		renderer.Clear(0);
		// renderer.DrawSky(*sky, player);
#if 1
		for (uint32_t x = 0; x < width; x++)
		{
			float angle = (player.GetAngle() - camera->GetFOV() / 2.0f) + (float)x / width * camera->GetFOV();

			char tex;
			glm::vec2 hit = camera->CastRay(player.GetPosition(), glm::vec2(glm::cos(angle), glm::sin(angle)), map, &tex);

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
#endif

		for (int i = enemies.size() - 1; i >= 0; i--)
		{
			renderer.DrawEnemy(*enemies[i], player);
		}

		renderer.DrawPlayer(player, (int)(-glm::sin(glfwGetTime() * 3) * 4), (int)(100 - glm::sin(glfwGetTime()) * 2));

		int error = glGetError();
		if (error)
			std::cout << "OpenGL ERROR: " << error << std::endl;

		renderer.Flush();

		window.Update();
		fps++;
	}
	// delete sky;
	return 0;
}