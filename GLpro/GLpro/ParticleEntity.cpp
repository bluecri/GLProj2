#include "ParticleEntity.h"

#include "RenderManager.h"
#include "ParticleFObj.h"
#include "ParticleFObjManager.h"
#include "ParticleCreateInfo.h"

#include "ParticleFObjManager.h"

ParticleEntity::ParticleEntity(GameSession * gSession, const char* textureFileName, const char* textureType, SHADER::ShaderParticle * shaderParticle)
	: Entity(gSession, ENUM_ENTITY_TYPE::ENUM_ENTITY_PARTICLE)
{
	// particle
	_rParticle = GRendermanager->getRRender<RENDER::RParticle, SHADER::ShaderParticle>(shaderParticle);
	_rElemInParticle = _rParticle->addToDrawList(GParticleFObjManager->getSharedParticleFObj("data/Texture/particle.DDS", "dds"), _rigidbodyComponent);
}

ParticleEntity::~ParticleEntity()
{
	_rElemInParticle->setDeleted();
	//_rElemInParticle->setDeleteRemainTime(2.0f);
}

void ParticleEntity::init(glm::vec3& localPos, glm::quat& localQuat, glm::vec3 mainDir, bool bOneParticlePerMultiFrame, const float& particleLife, const float& frameVsParticle)
{
	_rigidbodyComponent->setModelMatrix(localPos);
	_rigidbodyComponent->setQuaternion(localQuat);

	_rElemInParticle->_mainDir = mainDir;
	_rElemInParticle->_isOneParticlePerMultiFrame = bOneParticlePerMultiFrame;
	_rElemInParticle->_particleLife = particleLife;
	_rElemInParticle->_frameVsParticle = frameVsParticle;
}
void ParticleEntity::setFrameVsParticle(int frameVsParticle)
{
	_rElemInParticle->_frameVsParticle = frameVsParticle;
}

void ParticleEntity::setBRender(bool bRender)
{
	_rElemInParticle->setRender(bRender);
}

void ParticleEntity::setCollisionTest(bool bCollision)
{
	return;
}

void ParticleEntity::doJobWithBeDeleted()
{
	
}

void ParticleEntity::logicUpdate(float deltaTime, float acc)
{
}
