#pragma once

#include <string>

#include <gorilla/ga.h>
#include <gorilla/gau.h>

class Sound
{
public:
	Sound(const std::string& filename);
	~Sound();

private:
	friend class SoundManager;
	ga_Sound* m_Sound;
	ga_Handle* m_Handle;
	gau_SampleSourceLoop* m_LoopSrc;
};