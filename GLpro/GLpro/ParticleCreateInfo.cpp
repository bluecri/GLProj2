#include "./ParticleCreateInfo.h"
#include "./ParticleFObj.h"
#include "./src/Transform.h"

ParticleCreateInfo::ParticleCreateInfo(bool _isOneParticlePerMultiFrame, int _frameVsParticle, float life, float _spread, float _particleSizeStartRange, float _particleSizeEndRange)
	: _isOneParticlePerMultiFrame(_isOneParticlePerMultiFrame), _frameVsParticle(_frameVsParticle), _particleLife(life), _spread(_spread)
{
	_countFrame = 0;

	_mainDir = glm::vec3(0.0f, 3.0f, 0.0f);

	m_colorStartRange[0] = 0;
	m_colorStartRange[1] = 0;
	m_colorStartRange[2] = 0;
	m_colorStartRange[3] = 40;

	m_colorEndRange[0] = 255;
	m_colorEndRange[1] = 255;
	m_colorEndRange[2] = 255;
	m_colorEndRange[3] = 160;
}

ParticleCreateInfo::ParticleCreateInfo(int colorStartRange[4], int colorEndRange[4], glm::vec3 _mainDir, bool _isOneParticlePerMultiFrame = true, int _frameVsParticle = 5, float life = 3.0f, float _spread = 0.5f, float _particleSizeStartRange = 0.05f, float _particleSizeEndRange = 0.1f)
	: _isOneParticlePerMultiFrame(_isOneParticlePerMultiFrame), _frameVsParticle(_frameVsParticle), _particleLife(life), _spread(_spread), _mainDir(_mainDir), _particleSizeStartRange(_particleSizeStartRange), _particleSizeEndRange(_particleSizeEndRange) 
{
	_countFrame = 0;

	for (int i = 0; i < 4; i++)
	{
		m_colorStartRange[i] = colorStartRange[i];
		m_colorEndRange[i] = colorEndRange[i];
	}
}

void ParticleCreateInfo::genNewParticles(RENDER_TARGET::PARTICLE::ParticleFObj *particleFObj)
{
	if (_isOneParticlePerMultiFrame)		// draw when particle interval is matched
	{
		if (_countFrame >= _frameVsParticle)
		{
			_countFrame = 0;
			ParticleStruct& pStruct = particleFObj->GetUnusedParticle();
			crateNewParticleStructWithInfo(pStruct);
		}
		_countFrame++;
	}
	else 
	{
		// draw multiple particle in 1 frame
		for (int i = 0; i < _frameVsParticle; i++)
		{
			ParticleStruct& pStruct = particleFObj->GetUnusedParticle();
			crateNewParticleStructWithInfo(pStruct);
		}
	}
	
	return;
}

void ParticleCreateInfo::crateNewParticleStructWithInfo(ParticleStruct & refP)
{
	refP._life = _particleLife;
	refP._pos = _bindedTransform->getModelVec();

	glm::vec3 randomdir = glm::vec3(
		(rand() % 2000 - 1000.0f) / 1000.0f,
		(rand() % 2000 - 1000.0f) / 1000.0f,
		(rand() % 2000 - 1000.0f) / 1000.0f
	);
	refP._velocity = _mainDir + randomdir * _spread;


	for (int i = 0; i < 4; i++) {
		refP._color[i] = (unsigned char)(m_colorStartRange[i] > m_colorEndRange[i]) ? glm::linearRand(m_colorEndRange[i], m_colorStartRange[i]) : glm::linearRand(m_colorStartRange[i], m_colorEndRange[i]);
	}

	refP._size = glm::linearRand(_particleSizeStartRange, _particleSizeEndRange);

	return;
}