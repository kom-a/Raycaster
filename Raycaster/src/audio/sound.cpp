#include "sound.h"

Sound::Sound(const std::string& filename)
	: m_LoopSrc(0)
{
	int dotIndex = filename.find('.');
	std::string extension = filename.substr(dotIndex + 1, filename.length() - 1);
	m_Sound = gau_load_sound_file(filename.c_str(), extension.c_str());
}

Sound::~Sound()
{
	ga_sound_release(m_Sound);
}
