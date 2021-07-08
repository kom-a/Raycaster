#pragma once

#include <unordered_map>
#include <string>

#include <gorilla/ga.h>
#include <gorilla/gau.h>

#include "sound.h"

class SoundManager
{
public:
	static void Init();
	static void Load(const std::string& filename, const std::string& name);
	static void Update();

	static void Play(const std::string& name);
	static void Stop(const std::string& name);
	static void Loop(const std::string& name);
	static void StopAll();
	

private:
	static Sound* GetSound(const std::string& name);
private:
	SoundManager();

private:
	static gau_Manager* m_Manager;
	static ga_Mixer* m_Mixer;

	static std::unordered_map<std::string, Sound*> m_Sounds;
};
