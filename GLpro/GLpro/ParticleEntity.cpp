#include "ParticleEntity.h"

#include "RenderManager.h"
#include "ParticleFObj.h"
#include "ParticleCreateInfo.h"

ParticleEntity::ParticleEntity(GameSession * gSession, SHADER::ShaderParticle * shaderParticle)
	: Entity(gSession, ENUM_ENTITY_TYPE::ENUM_ENTITY_PARTICLE)
{
	// particle
	_rParticle = GRendermanager->getRRender<RENDER::RParticle, SHADER::ShaderParticle>(shaderParticle);
	_rElemInParticle = _rParticle->addToDrawList(new RENDER_TARGET::PARTICLE::ParticleFObj("data/Texture/particle.DDS", "dds"), _rigidbodyComponent);
}

ParticleEntity::~ParticleEntity()
{
}

void ParticleEntity::init(glm::vec3& localPos, glm::quat& localQuat, glm::vec3 mainDir, bool bOneParticlePerMultiFrame, const float& particleLife, const float& frameVsParticle)
{
	_rigidbodyComponent->setModelMatrix(localPos);
	_rigidbodyComponent->setQuaternion(localQuat);

	_rElemInParticle->second->_mainDir = mainDir;
	_rElemInParticle->second->_isOneParticlePerMultiFrame = bOneParticlePerMultiFrame;
	_rElemInParticle->second->_particleLife = particleLife;
	_rElemInParticle->second->_frameVsParticle = frameVsParticle;
}
void ParticleEntity::setFrameVsParticle(int frameVsParticle)
{
	_rElemInParticle->second->_frameVsParticle = frameVsParticle;
}

void ParticleEntity::setBRender(bool bRender)
{
	_rElemInParticle->first->setBRender(bRender);
}

void ParticleEntity::setCollisionTest(bool bCollision)
{
	return;
}

void ParticleEntity::doJobWithBeDeleted()
{
	_rElemInParticle->first->setBDeleted();
	_rElemInParticle->first->setDeleteRemainTime(-1.0f);
}

void ParticleEntity::logicUpdate(float deltaTime, float acc)
{
}
