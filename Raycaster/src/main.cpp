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
#include "enemies/mushroom.h"
#include "enemies/skeleton.h"

#include <glad/glad.h>

int main()
{
	uint32_t width = 640, height = width * 9 / 16;

	Window window(width, height, "Raycaster");
	Renderer renderer(width, height);

	Player player(17.1f, 5.0f, 3.1415f, "res/2.png", 250, 250);
	const Camera* camera = player.GetCamera();

	SpriteSheet sheet("res/spritesheet.bmp", 64, 64, false);
	 //const Texture* sky = FileManager::LoadTexture("res/Nebula Blue.png");
	const Texture* sky = FileManager::LoadTexture("res/sky2.jpg");
	Map map("res/maps/map2.rcm");

	ResourceManager::LoadSpriteSheet("res/Goblin/Idle.png", 33, 36, true, "GoblinIdleSheet");
	ResourceManager::LoadSpriteSheet("res/Goblin/Run.png", 36, 36, true, "GoblinRunSheet");
	ResourceManager::LoadSpriteSheet("res/Goblin/Attack.png", 50, 48, false, "GoblinAttackSheet");
	ResourceManager::LoadSpriteSheet("res/Goblin/TakeHit.png", 42, 37, false, "GoblinTakeHitSheet");
	ResourceManager::LoadSpriteSheet("res/Goblin/Death.png", 58, 40, false, "GoblinDeathSheet");

	ResourceManager::LoadSpriteSheet("res/FlyingEye/Idle.png", 42, 32, true, "FlyingEyeIdleSheet");
	ResourceManager::LoadSpriteSheet("res/FlyingEye/TakeHit.png", 42, 32, false, "FlyingEyeTakeHitSheet");
	ResourceManager::LoadSpriteSheet("res/FlyingEye/Death.png", 56, 40, false, "FlyingEyeDeathSheet");
	ResourceManager::LoadSpriteSheet("res/FlyingEye/Attack.png", 42, 32, false, "FlyingEyeAttackSheet");

	ResourceManager::LoadSpriteSheet("res/Mushroom/Idle.png", 24, 38, true, "MushroomIdleSheet");
	ResourceManager::LoadSpriteSheet("res/Mushroom/Run.png", 26, 38, true, "MushroomRunSheet");
	ResourceManager::LoadSpriteSheet("res/Mushroom/TakeHit.png", 38, 42, false, "MushroomTakeHitSheet");
	ResourceManager::LoadSpriteSheet("res/Mushroom/Death.png", 28, 38, false, "MushroomDeathSheet");
	ResourceManager::LoadSpriteSheet("res/Mushroom/Attack.png", 50, 46, false, "MushroomAttackSheet");

	ResourceManager::LoadSpriteSheet("res/Skeleton/Idle.png", 46, 51, true, "SkeletonIdleSheet");
	ResourceManager::LoadSpriteSheet("res/Skeleton/Run.png", 50, 50, true, "SkeletonRunSheet");
	ResourceManager::LoadSpriteSheet("res/Skeleton/TakeHit.png", 50, 52, false, "SkeletonTakeHitSheet");
	ResourceManager::LoadSpriteSheet("res/Skeleton/Death.png", 58, 52, false, "SkeletonDeathSheet");
	ResourceManager::LoadSpriteSheet("res/Skeleton/Attack.png", 85, 55, false, "SkeletonAttackSheet");

	std::vector<Enemy*> enemies;
	for (int i = 0; i < 12; i++)
	{
		int j = i % 3;
		if (i < 3)
			enemies.push_back(new Goblin(glm::vec2(3, 3 + i)));
		else if (i < 6)
			enemies.push_back(new Skeleton(glm::vec2(3, 3 + i)));
		else if(i < 9)
			enemies.push_back(new Mushroom(glm::vec2(3, 3 + i)));
		else
			enemies.push_back(new FlyingEye(glm::vec2(3, 3 + i)));
	}

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
			e->Update(deltaTime, player, map);
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
		renderer.DrawSky(*sky, player);
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
		std::vector<MagicBall*> bullets;
		for (int i = enemies.size() - 1; i >= 0; i--)
		{
			if (FlyingEye* fe = dynamic_cast<FlyingEye*>(enemies[i]))
			{
				const std::vector<MagicBall*> src = fe->GetBullets();
				bullets.insert(bullets.end(), std::make_move_iterator(src.begin()), std::make_move_iterator(src.end()));
			}
			renderer.DrawEnemy(*enemies[i], player);
		}

		std::sort(bullets.begin(), bullets.end(),
			[&](const MagicBall* a, const MagicBall* b) -> bool
			{
				// TODO: Optimize this
				float len1 = glm::length(player.GetPosition() - a->GetPosition());
				float len2 = glm::length(player.GetPosition() - b->GetPosition());
				return len1 < len2;
			});

		for (int i = bullets.size() - 1; i >= 0; i--)
		{
			if (bullets[i]->IsDestroyed()) continue;
			renderer.DrawSprite(bullets[i]->GetSprite(), player);
		}

		renderer.DrawPlayer(player, (int)(-glm::sin(glfwGetTime() * 3) * 4), (int)(100 - glm::sin(glfwGetTime()) * 2));
		renderer.DrawHUD(player);
		
		int error = glGetError();
		if (error)
			std::cout << "OpenGL ERROR: " << error << std::endl;

		
		renderer.Flush();

		window.Update();
		fps++;
	}
	delete sky;
	return 0;
}