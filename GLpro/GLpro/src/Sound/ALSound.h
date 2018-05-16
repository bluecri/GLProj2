#pragma once
#include <al.h>
#include <alc.h>

#include <stdio.h>
#include <iostream>

using namespace std;

/*
 * contain soundBufferID & information of file
*/
class ALSound {
public:
	ALuint soundBufferID;
	std::string m_soundName;
	std::string fileName;
	ALvoid	*data = 0;
	ALsizei size = 0;
	ALenum format = 0;
	ALsizei freq = 0;
	
	ALSound(std::string fileName, ALvoid *data, ALsizei size, ALenum format, ALsizei freq);

	void allocBuffer(std::string soundName);

	virtual ~ALSound();
};