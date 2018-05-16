#include "../../stdafx.h"
#include "ALSource.h"

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
	alSourcei(m_sourceID, AL_MAX_DISTANCE, 10.0f);
	alSourcei(m_sourceID, AL_REFERENCE_DISTANCE, 1.0);
}

ALSource::ALSource(Transform* transform, float pitch = 1.0f, float gain = 1.0f) 
	: m_pitch(pitch), m_gain(gain), _transform(transform)
{
	alGenSources(1, &m_sourceID);

	updatePos();

	m_isLoop = AL_FALSE;

	alSourcef(m_sourceID, AL_PITCH, m_pitch);
	alSourcef(m_sourceID, AL_GAIN, m_gain);
	alSourcefv(m_sourceID, AL_POSITION, m_sourcePos);
	alSourcei(m_sourceID, AL_LOOPING, m_isLoop);
	alSourcei(m_sourceID, AL_MAX_DISTANCE, 10.0f);
	alSourcei(m_sourceID, AL_REFERENCE_DISTANCE, 1.0);
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

void ALSource::updatePos() {
	glm::vec3& modelVec = _transform->getModelVec();
	m_sourcePos[0] = modelVec[0];
	m_sourcePos[1] = modelVec[1];
	m_sourcePos[2] = modelVec[2];

	alSourcefv(m_sourceID, AL_POSITION, m_sourcePos);
}

void ALSource::sourcePlay() {
	alSourcePlay(m_sourceID);
}

void ALSource::sourceStop() {
	alSourceStop(m_sourceID);
}

ALSource::~ALSource() {
	alDeleteSources(1, &m_sourceID);
}
