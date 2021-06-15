#define _USE_MATH_DEFINES

#include <iostream>
#include <cstdlib>
#include <vector>
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

	Player player(6.0f, 3.0f, 0.0f, "res/2.png", 250, 250);
	Camera camera((float)M_PI / 3.0f);

	SpriteSheet sheet("res/spritesheet.bmp", 64, 64, false);
	const Texture* sky = FileManager::LoadTexture("res/Nebula Blue.png");
	Map map("res/maps/textureTestMap.rcm");

	ResourceManager::LoadSpriteSheet("res/Goblin/Idle.png", 33, 36, true, "GoblinIdleSheet");
	ResourceManager::LoadSpriteSheet("res/Goblin/Attack.png", 90, 48, false, "GoblinAttackSheet");
	ResourceManager::LoadSpriteSheet("res/Goblin/TakeHit.png", 42, 37, false, "GoblinTakeHitSheet");
	ResourceManager::LoadSpriteSheet("res/Goblin/Death.png", 58, 40, false, "GoblinDeathSheet");

	//Sprite sprite("res/tronchungo3.png", 3, 4);
	//Sprite sprite2("res/Warrior_Idle_1.png", 5, 2);
	//Sprite sprite3("res/sorcerer attack_Animation 1_0.png", 6, 6);

	srand(time(NULL));
	std::vector<Enemy> enemies;
	for (int i = 0; i < 0; i++)
	{
		if(i < 6)
			enemies.push_back(Goblin(glm::vec2(3.0f, i * 2)));
		else
			enemies.push_back(FlyingEye(glm::vec2(3.0f, i * 2)));
	}
	enemies.push_back(FlyingEye(glm::vec2(3, 3)));

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

		player.Update(deltaTime, map, enemies);
		for (Enemy& e : enemies)
		{
			e.Update(deltaTime);
		}


		renderer.Clear();
		renderer.DrawSky(*sky, player);
#if 1
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
#endif

		/*renderer.DrawSprite(sprite, player);
		renderer.DrawSprite(sprite2, player);
		renderer.DrawSprite(sprite3, player);*/
		for (Enemy& e : enemies)
		{
			renderer.DrawEnemy(e, player);
		}



		renderer.DrawPlayer(player, -glm::sin(glfwGetTime() * 3) * 4 , 100 - glm::sin(glfwGetTime()) * 2);
		
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