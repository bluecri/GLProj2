#pragma once

#include "./stdafx.h"

namespace RENDER_TARGET {
	namespace PARTICLE {
		class ParticleFObj;
	}
}
class RigidbodyComponent;
typedef struct _ParticleStruct ParticleStruct;

class ParticleCreateInfo
{
public:
	//ParticleCreateInfo(bool _isOneParticlePerMultiFrame = true, int _frameVsParticle = 5, float life = 3.0f, float _spread = 0.5f, float _particleSizeStartRange = 0.05f, float _particleSizeEndRange = 0.1f);
	ParticleCreateInfo(bool _isOneParticlePerMultiFrame, std::shared_ptr<RENDER_TARGET::PARTICLE::ParticleFObj> fObj, int _frameVsParticle = 5, float life = 3.0f, float _spread = 0.5f, float _particleSizeStartRange = 0.05f, float _particleSizeEndRange = 0.1f);
	ParticleCreateInfo(std::shared_ptr<RENDER_TARGET::PARTICLE::ParticleFObj> fObj, int colorStartRange[4], int colorEndRange[4], glm::vec3 _mainDir, bool _isOneParticlePerMultiFrame, int _frameVsParticle, float life, float _spread, float _particleSizeStartRange = 0.05f, float _particleSizeEndRange = 0.1f);
	
	void init(RigidbodyComponent* bindedRigidbodyComponent);
	void genNewParticles();	// Generate multiple particles with FObj

	bool isRender();
	void setRender(bool bRender);
	bool isDeleted();
	void setDeleted();


	void				setDeleteRemainTime(float remainTime);
	float&				getDeleteRemainTimeRef();


	std::shared_ptr<RENDER_TARGET::PARTICLE::ParticleFObj> getSharedParticleFObj();
private:
	ParticleCreateInfo() = delete;
	void crateNewParticleStructWithInfo(ParticleStruct& refP);	// Create particleStruct with *this info.

public:

	bool		_isOneParticlePerMultiFrame;
	int			_frameVsParticle = 1;		// for multi frame per 1 particle
	float		_particleLife;
	float		_spread;
	glm::vec3	_mainDir;

	int			m_colorStartRange[4],		m_colorEndRange[4];
	float		_particleSizeStartRange,	_particleSizeEndRange;

	bool		_bRender;
	bool		_bDeleted;

private:
	std::shared_ptr<RENDER_TARGET::PARTICLE::ParticleFObj>	_sharedParticleFObj;

	RigidbodyComponent* _bindedRigidbodyComponent;		// for create Particleinfo
	int			_countFrame;		//frame count

	float		_deleteRemainTime;

};