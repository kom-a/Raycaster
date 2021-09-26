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

#include "audio/sound_manager.h"

#include <glad/glad.h>

enum class GAME_STATE
{
	MENU,
	GAME,
	WIN,
	LOSE,
};

int main()
{
	GAME_STATE state = GAME_STATE::MENU;
	uint32_t width = 640, height = width * 9 / 16;

	Window window(width, height, "Raycaster");
	Renderer renderer(width, height);

	Player player(56.5f, 14.5f, 2 * 3.1415f, "res/2.png", 250, 250);
	// Player player(4, 3, 3.1415f, "res/2.png", 250, 250);
	const Camera* camera = player.GetCamera();

	SpriteSheet sheet("res/wolftextures.png", 64, 64, false);
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

	ResourceManager::LoadSpriteSheet("res/Font.png", 10, 10, false, "Font");

	SoundManager::Init();
	SoundManager::Load("res/audio/Shot.wav", "shot");
	SoundManager::Load("res/audio/xDeviruchi - The Icy Cave .wav", "mainTheme");
	SoundManager::Load("res/audio/xDeviruchi - Take some rest and eat some food!.wav", "win");
	SoundManager::Load("res/audio/xDeviruchi - The Final of The Fantasy.wav", "lost");

	//SoundManager::Loop("mainTheme");

	Sprite princess("res/princess.png", 3, 3, 0.8f, true);

	std::vector<Enemy*> enemies;

#if 1
	{
		enemies.push_back(new Goblin(glm::vec2(25.25f, 2.40f)));
		enemies.push_back(new Goblin(glm::vec2(24.00f, 6.00f)));
		enemies.push_back(new Goblin(glm::vec2(24.00f, 7.25f)));
		enemies.push_back(new Goblin(glm::vec2(29.50f, 8.40f)));
		enemies.push_back(new Goblin(glm::vec2(33.30f, 8.40f)));
		enemies.push_back(new Goblin(glm::vec2(37.30f, 8.40f)));
		enemies.push_back(new FlyingEye(glm::vec2(50.90f, 8.40f)));
		enemies.push_back(new FlyingEye(glm::vec2(50.90f, 9.40f)));
		enemies.push_back(new FlyingEye(glm::vec2(50.90f, 10.40f)));
		enemies.push_back(new Skeleton(glm::vec2(44.80f, 5.50f)));
		enemies.push_back(new Skeleton(glm::vec2(44.80f, 6.50f)));
		enemies.push_back(new FlyingEye(glm::vec2(28.00f, 2.50f)));
		enemies.push_back(new FlyingEye(glm::vec2(28.00f, 3.50f)));
		enemies.push_back(new FlyingEye(glm::vec2(28.00f, 4.50f)));
		enemies.push_back(new FlyingEye(glm::vec2(28.00f, 5.50f)));
		enemies.push_back(new Goblin(glm::vec2(29.00f, 2.50f)));
		enemies.push_back(new Goblin(glm::vec2(29.00f, 3.50f)));
		enemies.push_back(new Goblin(glm::vec2(29.00f, 4.50f)));
		enemies.push_back(new Goblin(glm::vec2(29.00f, 5.50f)));
		enemies.push_back(new Skeleton(glm::vec2(35.00f, 1.50f)));
		enemies.push_back(new Skeleton(glm::vec2(35.00f, 2.50f)));
		enemies.push_back(new Skeleton(glm::vec2(35.00f, 3.50f)));
		enemies.push_back(new FlyingEye(glm::vec2(45.00f, 1.50f)));
		enemies.push_back(new FlyingEye(glm::vec2(45.00f, 2.50f)));
		enemies.push_back(new FlyingEye(glm::vec2(45.00f, 3.50f)));
		enemies.push_back(new Mushroom(glm::vec2(40.00f, 1.50f)));
		enemies.push_back(new Mushroom(glm::vec2(40.00f, 2.50f)));
		enemies.push_back(new Mushroom(glm::vec2(40.00f, 3.50f)));
		enemies.push_back(new Mushroom(glm::vec2(51.50f, 6.50f)));
		enemies.push_back(new Mushroom(glm::vec2(51.50f, 4.50f)));
		enemies.push_back(new Goblin(glm::vec2(47.50f, 6.50f)));
		enemies.push_back(new Goblin(glm::vec2(48.50f, 6.50f)));
		enemies.push_back(new Goblin(glm::vec2(49.50f, 6.50f)));
		enemies.push_back(new Goblin(glm::vec2(50.50f, 6.50f)));
		enemies.push_back(new FlyingEye(glm::vec2(60.50f, 1.50f)));
		enemies.push_back(new FlyingEye(glm::vec2(60.50f, 2.50f)));
		enemies.push_back(new FlyingEye(glm::vec2(60.50f, 3.50f)));
		enemies.push_back(new FlyingEye(glm::vec2(60.50f, 4.50f)));
		enemies.push_back(new FlyingEye(glm::vec2(60.50f, 5.50f)));
		enemies.push_back(new Mushroom(glm::vec2(56.50f, 11.50f)));
		enemies.push_back(new Mushroom(glm::vec2(57.50f, 11.50f)));
		enemies.push_back(new Mushroom(glm::vec2(58.50f, 11.50f)));
		enemies.push_back(new Skeleton(glm::vec2(53.50f, 10.50f)));
		enemies.push_back(new Skeleton(glm::vec2(54.50f, 10.50f)));
		enemies.push_back(new Skeleton(glm::vec2(55.50f, 10.50f)));
		enemies.push_back(new Goblin(glm::vec2(58.50f, 4.50f)));
		enemies.push_back(new Goblin(glm::vec2(58.50f, 5.50f)));
		enemies.push_back(new Goblin(glm::vec2(58.50f, 6.50f)));
		enemies.push_back(new Mushroom(glm::vec2(54.50f, 12.50f)));
		enemies.push_back(new Mushroom(glm::vec2(55.50f, 12.50f)));
		enemies.push_back(new Mushroom(glm::vec2(42.50f, 14.50f)));
		enemies.push_back(new Mushroom(glm::vec2(42.50f, 12.50f)));
		enemies.push_back(new FlyingEye(glm::vec2(29.50f, 14.50f)));
		enemies.push_back(new Goblin(glm::vec2(19.50f, 14.50f)));
		enemies.push_back(new Goblin(glm::vec2(21.50f, 14.50f)));
		enemies.push_back(new Goblin(glm::vec2(25.50f, 14.50f)));
		enemies.push_back(new Skeleton(glm::vec2(5.50f, 14.50f)));
		enemies.push_back(new Skeleton(glm::vec2(2.50f, 6.50f)));
		enemies.push_back(new Skeleton(glm::vec2(3.50f, 6.50f)));
		enemies.push_back(new Skeleton(glm::vec2(4.50f, 6.50f)));
		enemies.push_back(new Skeleton(glm::vec2(5.50f, 6.50f)));
		enemies.push_back(new Goblin(glm::vec2(3.75f, 9.50f)));
		enemies.push_back(new Goblin(glm::vec2(3.75f, 10.50f)));
		enemies.push_back(new Goblin(glm::vec2(3.75f, 11.50f)));
		enemies.push_back(new Goblin(glm::vec2(3.75f, 12.50f)));
		enemies.push_back(new Mushroom(glm::vec2(1.50f, 8.50f)));
		enemies.push_back(new Mushroom(glm::vec2(1.50f, 12.50f)));
		enemies.push_back(new Mushroom(glm::vec2(10.50f, 9.50f)));
		enemies.push_back(new FlyingEye(glm::vec2(7.50f, 1.50f)));
		enemies.push_back(new FlyingEye(glm::vec2(8.50f, 1.50f)));
		enemies.push_back(new FlyingEye(glm::vec2(9.50f, 1.50f)));
		enemies.push_back(new FlyingEye(glm::vec2(10.50f, 1.50f)));
		enemies.push_back(new FlyingEye(glm::vec2(11.50f, 1.50f)));
		enemies.push_back(new Skeleton(glm::vec2(7.50f, 4.50f)));
		enemies.push_back(new Goblin(glm::vec2(8.50f, 4.50f)));
		enemies.push_back(new Goblin(glm::vec2(9.50f, 4.50f)));
		enemies.push_back(new Skeleton(glm::vec2(10.50f, 4.50f)));
		enemies.push_back(new Skeleton(glm::vec2(9.50f, 8.50f)));
		enemies.push_back(new Skeleton(glm::vec2(9.50f, 7.50f)));
	}
#endif

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

		if (Keyboard::IsKeyPressed(' '))
		{
			std::cout << player.GetPosition().x << "\t" << player.GetPosition().y << std::endl;
		}

		switch (state)
		{
		case GAME_STATE::MENU:
		{
			window.DisableCursor(false);
			renderer.FillRect(0, 0, width, height, glm::ivec3(200, 100, 50));
			renderer.DrawString(width / 2 - 50, height / 2 - 50, "Play", 24);
			renderer.DrawString(width / 2 - 50, height / 2 + 30, "Exit", 24);
			
			float mouseX = Mouse::GetX() / window.GetWidth();
			float mouseY = Mouse::GetY() / window.GetHeight();
			if (mouseX > 0.4f && mouseX < 0.6f && mouseY > 0.35f && mouseY < 0.45f)
			{
				renderer.FillRect(240, 140, 10, 10, glm::ivec3(0));
				if (Mouse::IsButtonPressed(0)) // Left mouse button pressed
				{
					state = GAME_STATE::GAME;
					SoundManager::StopAll();
					//SoundManager::Loop("mainTheme");
				}
			}
			else if (mouseX > 0.4f && mouseX < 0.6f && mouseY > 0.55f && mouseY < 0.7f)
			{
				renderer.FillRect(240, 220, 10, 10, glm::ivec3(0));
				if (Mouse::IsButtonPressed(0)) // Left mouse button pressed
				{ 
					window.Close();
				}
			}
			
		} break;
		case GAME_STATE::GAME:
		{
			window.DisableCursor(true);
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
			renderer.DrawSprite(princess, player);

			renderer.DrawPlayer(player, (int)(-glm::sin(glfwGetTime() * 3) * 4), (int)(100 - glm::sin(glfwGetTime()) * 2));
			renderer.DrawHUD(player);
			
			if (Keyboard::IsKeyPressed(GLFW_KEY_ESCAPE))
			{
				state = GAME_STATE::MENU;
				SoundManager::StopAll();
				//SoundManager::Loop("mainTheme");
			}
			
			if (glm::length(player.GetPosition() - princess.GetPosition()) < 1.0f)
			{
				state = GAME_STATE::WIN;
				SoundManager::StopAll();
				SoundManager::Play("win");
			}

			if (Keyboard::IsKeyPressed(' '))
			{
				std::cout << player.GetPosition().x << ", " << player.GetPosition().y << std::endl;
			}
			if (player.GetHealth() <= 0)
			{
				state = GAME_STATE::LOSE;
				SoundManager::StopAll();
				SoundManager::Play("lost");
			}
				

		} break;
		case GAME_STATE::WIN:
		{
			renderer.FillRect(0, 0, width, height, glm::ivec3(50, 200, 100));
			renderer.DrawString(width / 2 - 100, height / 2 - 50, "YOU WON", 32);
			renderer.DrawString(width / 2 - 85, height - 16, "Press ESCAPE TO EXIT", 10);

			if (Keyboard::IsKeyPressed(GLFW_KEY_ESCAPE))
				window.Close();
		} break;
		case GAME_STATE::LOSE:
		{
			renderer.FillRect(0, 0, width, height, glm::ivec3(200, 50, 50));
			renderer.DrawString(width / 2 - 100, height / 2 - 50, "YOU LOST", 32);
			renderer.DrawString(width / 2 - 85, height - 16, "Press ESCAPE TO EXIT", 10);

			if (Keyboard::IsKeyPressed(GLFW_KEY_ESCAPE))
				window.Close();
		} break;
		}

		int error = glGetError();
		if (error)
			std::cout << "OpenGL ERROR: " << error << std::endl;
		
		renderer.Flush();

		SoundManager::Update();
		window.Update();
		fps++;
	}
	delete sky;
	return 0;
}