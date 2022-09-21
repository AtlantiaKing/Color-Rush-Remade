#include "pch.h"
#include "Music.h"
#include "SoundStream.h"

Music::Music()
	: m_pMusic{}
{
}

Music::~Music()
{
	delete m_pMusic;
}

void Music::Load(const std::string& filePath, bool loop)
{
	delete m_pMusic;
	m_pMusic = new SoundStream{ filePath };
	m_pMusic->Play(loop);
}

bool Music::IsPlaying() const
{
	return m_pMusic->IsPlaying();
}
