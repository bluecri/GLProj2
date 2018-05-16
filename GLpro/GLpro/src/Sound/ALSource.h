#pragma once
#include <al.h>
#include <alc.h>

#include "./ALSound.h"
#include "../Transform.h"

class ALSource {

public:
	ALSource();

	ALSource(Transform* transform, float pitch, float gain);
	void initSource(ALuint soundBufferID);

	void bindSourceToBuffer(ALuint soundBufferID);
	void bindSourceToALSound(ALSound *alSound);

	void updatePos();		// with _transform pos

	void sourcePlay();
	void sourceStop();

	~ALSource();

public:
	ALuint m_sourceID;
	float m_pitch;
	float m_gain;
	float m_sourcePos[3];
	ALboolean m_isLoop = AL_FALSE;

	Transform* _transform;
};