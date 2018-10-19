#include "./ParticleCreateInfo.h"
#include "./ParticleFObj.h"

#include "RigidbodyComponent.h"
#include "ParticleStruct.h"

ParticleCreateInfo::ParticleCreateInfo(bool isOneParticlePerMultiFrame, std::shared_ptr<RENDER_TARGET::PARTICLE::ParticleFObj> fObj, int frameVsParticle, float life, float spread, float particleSizeStartRange, float particleSizeEndRange)
	: _isOneParticlePerMultiFrame(isOneParticlePerMultiFrame), _frameVsParticle(frameVsParticle), _particleLife(life), _spread(spread), _particleSizeStartRange(particleSizeStartRange), _particleSizeEndRange(particleSizeEndRange)
{
	_countFrame = 0;
	_sharedParticleFObj = fObj;
	_bRender = true;
	_bDeleted = false;

	_mainDir = glm::vec3(0.0f, 0.0f, -1.0f);

	m_colorStartRange[0] = 0;
	m_colorStartRange[1] = 0;
	m_colorStartRange[2] = 0;
	m_colorStartRange[3] = 100;

	m_colorEndRange[0] = 255;
	m_colorEndRange[1] = 255;
	m_colorEndRange[2] = 255;
	m_colorEndRange[3] = 200;
}

ParticleCreateInfo::ParticleCreateInfo(std::shared_ptr<RENDER_TARGET::PARTICLE::ParticleFObj> fObj, int colorStartRange[4], int colorEndRange[4], glm::vec3 _mainDir, bool _isOneParticlePerMultiFrame = true, int _frameVsParticle = 5, float life = 3.0f, float _spread = 0.5f, float _particleSizeStartRange, float _particleSizeEndRange)
	: _isOneParticlePerMultiFrame(_isOneParticlePerMultiFrame), _frameVsParticle(_frameVsParticle), _particleLife(life), _spread(_spread), _mainDir(_mainDir), _particleSizeStartRange(_particleSizeStartRange), _particleSizeEndRange(_particleSizeEndRange) 
{
	_countFrame = 0;
	_sharedParticleFObj = fObj;
	_bRender = true;
	_bDeleted = false;

	for (int i = 0; i < 4; i++)
	{
		m_colorStartRange[i] = colorStartRange[i];
		m_colorEndRange[i] = colorEndRange[i];
	}
}

void ParticleCreateInfo::init(RigidbodyComponent * bindedRigidbodyComponent)
{
	_bindedRigidbodyComponent = bindedRigidbodyComponent;
}

void ParticleCreateInfo::genNewParticles()
{
	if (_sharedParticleFObj == nullptr)
		return; 

	if (_isOneParticlePerMultiFrame)		// draw when particle interval is matched
	{
		if (_countFrame >= _frameVsParticle)
		{
			_countFrame = 0;
			ParticleStruct& pStruct = _sharedParticleFObj->GetUnusedParticle();
			crateNewParticleStructWithInfo(pStruct);
		}
		_countFrame++;
	}
	else 
	{
		// draw multiple particle in 1 frame
		for (int i = 0; i < _frameVsParticle; i++)
		{
			ParticleStruct& pStruct = _sharedParticleFObj->GetUnusedParticle();
			crateNewParticleStructWithInfo(pStruct);
		}
	}
	
	return;
}

// Generate multiple particles with FObj

bool ParticleCreateInfo::isRender()
{
	return _bRender;
}

void ParticleCreateInfo::setRender(bool bRender)
{
	_bRender = bRender;
}

bool ParticleCreateInfo::isDeleted()
{
	return _bDeleted;
}

void ParticleCreateInfo::setDeleted()
{
	_bDeleted = true;
	_bindedRigidbodyComponent = nullptr;
}

void ParticleCreateInfo::setDeleteRemainTime(float remainTime)
{
	_deleteRemainTime = remainTime;
}

float & ParticleCreateInfo::getDeleteRemainTimeRef()
{
	return _deleteRemainTime;
}

// Generate multiple particles to FObj

std::shared_ptr<RENDER_TARGET::PARTICLE::ParticleFObj> ParticleCreateInfo::getSharedParticleFObj()
{
	return _sharedParticleFObj;
}

void ParticleCreateInfo::crateNewParticleStructWithInfo(ParticleStruct & refP)
{
	if (_bindedRigidbodyComponent == nullptr)
		return;

	refP._life = _particleLife;
	refP._pos = _bindedRigidbodyComponent->getWorldPosVec();

	glm::vec3 randomdir = glm::vec3(
		(rand() % 2000 - 1000.0f) / 1000.0f,
		(rand() % 2000 - 1000.0f) / 1000.0f,
		(rand() % 2000 - 1000.0f) / 1000.0f
	);

	glm::mat3 worldRotMat = glm::mat3(_bindedRigidbodyComponent->getWorldMatRef());
	refP._velocity = worldRotMat * _mainDir + randomdir * _spread;


	for (int i = 0; i < 4; i++) {
		refP._color[i] = (unsigned char)(m_colorStartRange[i] > m_colorEndRange[i]) ? glm::linearRand(m_colorEndRange[i], m_colorStartRange[i]) : glm::linearRand(m_colorStartRange[i], m_colorEndRange[i]);
	}

	refP._size = glm::linearRand(_particleSizeStartRange, _particleSizeEndRange);

	return;
}
