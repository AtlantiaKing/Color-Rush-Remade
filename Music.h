#pragma once

class SoundStream;

class Music final
{
public:
	Music();
	~Music();

	void Load(const std::string& filePath, bool loop);
	bool IsPlaying() const;
private:
	SoundStream* m_pMusic;
};

