#include "sound_manager.h"

#include <iostream>

gau_Manager* SoundManager::m_Manager = nullptr;
ga_Mixer* SoundManager::m_Mixer = nullptr;
std::unordered_map<std::string, Sound*> SoundManager::m_Sounds;

void SoundManager::Init()
{
	gc_initialize(0);
	m_Manager = gau_manager_create();
	m_Mixer = gau_manager_mixer(m_Manager);
}

Sound* SoundManager::GetSound(const std::string& name)
{
	if (m_Sounds.find(name) == m_Sounds.end())
	{
		std::cout << "Failed to find \'" << name << "\' sound" << std::endl;
		return nullptr;
	}

	return m_Sounds[name];
}

void SoundManager::Load(const std::string& filename, const std::string& name)
{
	Sound* sound = new Sound(filename);
	sound->m_Handle = gau_create_handle_sound(m_Mixer, sound->m_Sound, nullptr, nullptr, nullptr);
	m_Sounds[name] = sound;
}

void SoundManager::Update()
{
	gau_manager_update(m_Manager);
}

void SoundManager::Play(const std::string& name)
{
	Sound* sound = GetSound(name);
	sound->m_Handle = gau_create_handle_sound(m_Mixer, sound->m_Sound, nullptr, nullptr, nullptr);
	ga_handle_play(sound->m_Handle);
}

void SoundManager::Stop(const std::string& name)
{
	ga_handle_stop(GetSound(name)->m_Handle);
}

void SoundManager::Loop(const std::string& name)
{
	Sound* sound = GetSound(name);
	ga_handle_destroy(sound->m_Handle);
	sound->m_Handle = gau_create_handle_sound(m_Mixer, sound->m_Sound, nullptr, nullptr, &sound->m_LoopSrc);
	ga_handle_play(sound->m_Handle);
}

void SoundManager::StopAll()
{
	for (auto& it : m_Sounds)
	{
		Stop(it.first);
	}
}
