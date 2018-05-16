#include "../../stdafx.h"
#include "ALSound.h"

ALSound::ALSound(std::string fileName, ALvoid * data, ALsizei size, ALenum format, ALsizei freq)
	: data(data), size(size), format(format), freq(freq), fileName(fileName) {
}

void ALSound::allocBuffer(std::string soundName)
{
	alGenBuffers(1, &soundBufferID);
	m_soundName = soundName;
	alBufferData(soundBufferID, format, data, size, freq);
}

ALSound::~ALSound()
{
	alDeleteBuffers(1, &soundBufferID);
	//alDeleteSources(1, &sourceID);
	delete data;
}
