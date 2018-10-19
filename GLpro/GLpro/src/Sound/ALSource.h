#pragma once
#include <al.h>
#include <alc.h>

#include "./ALSound.h"

class ALManager;
class RigidbodyComponent;

class ALSource {

public:
	ALSource();

	ALSource(RigidbodyComponent* rigidbodyComponent, float pitch, float gain);
	void initSource(ALuint soundBufferID);
	void bindSourceToBuffer(ALuint soundBufferID);
	void bindSourceToALSound(ALSound *alSound);

	void updateRigidbodyComponentPtr(RigidbodyComponent * rigidbodyComponent);
	void play();
	void stop();

	void setDoDelete() { _bDoDeleted = true;}
	void unBind();
	~ALSource();

private:
	void sourcePlay();
	void sourceStop();
	void updatePos();		// with _RigidbodyComponent pos
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

	RigidbodyComponent* _rigidbodyComponent;
};