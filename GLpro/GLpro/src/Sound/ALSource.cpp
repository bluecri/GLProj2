#include "../../stdafx.h"
#include "ALSource.h"
#include "../../RigidbodyComponent.h"

ALSource::ALSource() {
	alGenSources(1, &m_sourceID);
	m_pitch = 1.0f;
	m_gain = 1.0f;
	for (int i = 0; i < 3; i++) {
		m_sourcePos[i] = 0.0f;
	}
	m_isLoop = AL_FALSE;

	alSourcef(m_sourceID, AL_PITCH, m_pitch);
	alSourcef(m_sourceID, AL_GAIN, m_gain);
	alSourcefv(m_sourceID, AL_POSITION, m_sourcePos);
	alSourcei(m_sourceID, AL_LOOPING, m_isLoop);
	alSourcei(m_sourceID, AL_MAX_DISTANCE, 10);
	alSourcei(m_sourceID, AL_REFERENCE_DISTANCE, 1);
}

ALSource::ALSource(RigidbodyComponent* rigidbodyComponent, float pitch = 1.0f, float gain = 1.0f)
	: m_pitch(pitch), m_gain(gain), _rigidbodyComponent(rigidbodyComponent)
{
	alGenSources(1, &m_sourceID);

	updatePos();

	m_isLoop = AL_FALSE;

	alSourcef(m_sourceID, AL_PITCH, m_pitch);
	alSourcef(m_sourceID, AL_GAIN, m_gain);
	alSourcefv(m_sourceID, AL_POSITION, m_sourcePos);
	alSourcei(m_sourceID, AL_LOOPING, m_isLoop);
	alSourcei(m_sourceID, AL_MAX_DISTANCE, 10);
	alSourcei(m_sourceID, AL_REFERENCE_DISTANCE, 1);
}

void ALSource::initSource(ALuint soundBufferID) {
	bindSourceToBuffer(soundBufferID);
}

void ALSource::bindSourceToBuffer(ALuint soundBufferID) {
	alSourcei(m_sourceID, AL_BUFFER, soundBufferID);
}

void ALSource::bindSourceToALSound(ALSound * alSound) {
	alSourcei(m_sourceID, AL_BUFFER, alSound->soundBufferID);
}

void ALSource::updateRigidbodyComponentPtr(RigidbodyComponent * rigidbodyComponent)
{
	_rigidbodyComponent = rigidbodyComponent;
}

void ALSource::play()
{
	_bDoPlay = true;
}

void ALSource::stop()
{
	_bDoStop = true;
}

void ALSource::updatePos() {
	if (_rigidbodyComponent == nullptr)
		return;
	const glm::vec3& posVec = _rigidbodyComponent->getWorldPosVec();
	m_sourcePos[0] = posVec[0];
	m_sourcePos[1] = posVec[1];
	m_sourcePos[2] = posVec[2];

	// can transfer posVec direct. (for save source pos)
	alSourcefv(m_sourceID, AL_POSITION, m_sourcePos);
}

void ALSource::sourcePlay() {
	alSourcePlay(m_sourceID);
}

void ALSource::sourceStop() {
	alSourceStop(m_sourceID);
}

void ALSource::unBind() {
	_rigidbodyComponent = nullptr;
}

ALSource::~ALSource() {
	alDeleteSources(1, &m_sourceID);
}
