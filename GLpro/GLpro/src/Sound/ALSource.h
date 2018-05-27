#pragma once
#include <al.h>
#include <alc.h>

#include "./ALSound.h"
#include "../Transform.h"

class ALManager;

class ALSource {

public:
	ALSource();

	ALSource(Transform* transform, float pitch, float gain);
	void initSource(ALuint soundBufferID);
	void bindSourceToBuffer(ALuint soundBufferID);
	void bindSourceToALSound(ALSound *alSound);

	void updateTransformPtr(Transform * transform)
	{
		_transform = transform;
	}
	void play()
	{
		_bDoPlay = true;
	}
	void stop()
	{
		_bDoStop = true;
	}

	void setDoDelete() { _bDoDeleted = true;}
	~ALSource();

private:
	void sourcePlay();
	void sourceStop();
	void updatePos();		// with _transform pos
	friend class ALManager;

private:
	bool _bDoDeleted;
	bool _bDoPlay;
	bool _bDoStop;

	ALuint m_sourceID;
	float m_pitch;
	float m_gain;
	float m_sourcePos[3];
	ALboolean m_isLoop = AL_FALSE;

	Transform* _transform;
};