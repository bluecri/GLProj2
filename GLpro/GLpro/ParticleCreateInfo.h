#pragma once

#include "./stdafx.h"
#include "ParticleStruct.h"

namespace RENDER_TARGET {
	namespace PARTICLE {
		class ParticleFObj;
	}
}
class Transform;

class ParticleCreateInfo
{
public:
	//ParticleCreateInfo(bool _isOneParticlePerMultiFrame = true, int _frameVsParticle = 5, float life = 3.0f, float _spread = 0.5f, float _particleSizeStartRange = 0.05f, float _particleSizeEndRange = 0.1f);
	ParticleCreateInfo(bool _isOneParticlePerMultiFrame, int _frameVsParticle = 5, float life = 3.0f, float _spread = 0.5f, float _particleSizeStartRange = 0.05f, float _particleSizeEndRange = 0.1f);
	ParticleCreateInfo(int colorStartRange[4], int colorEndRange[4], glm::vec3 _mainDir, bool _isOneParticlePerMultiFrame, int _frameVsParticle, float life, float _spread, float _particleSizeStartRange = 0.05f, float _particleSizeEndRange = 0.1f);
	void init(Transform* bindedTransform);
	void genNewParticles(RENDER_TARGET::PARTICLE::ParticleFObj *particleFObj);	// Generate multiple particles to FObj

private:
	ParticleCreateInfo() = delete;
	void crateNewParticleStructWithInfo(ParticleStruct& refP);	// Create particleStruct with *this info.

public:

	bool	_isOneParticlePerMultiFrame;
	int		_frameVsParticle = 1;		// for multi frame per 1 particle
	float	_particleLife;
	float	_spread;
	glm::vec3	_mainDir;

	int		m_colorStartRange[4], m_colorEndRange[4];
	float	_particleSizeStartRange, _particleSizeEndRange;

private:
	Transform* _bindedTransform;		// for create Particleinfo
	int _countFrame;		//frame count

};