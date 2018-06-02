#pragma once

#include "./IPlane.h"
#include "./RParticle.h"

namespace SHADER { class ShaderMain; }

class ALSource;
class MissileGeneratorStorage;

class ParticleEntity : public Entity
{
public:
	ParticleEntity(GameSession* gSession, SHADER::ShaderParticle * shaderParticle);
	virtual ~ParticleEntity();
	void init(glm::vec3& localPos, glm::quat& localQuat, glm::vec3 mainDir, bool bOneParticlePerMultiFrame, const float& particleLife, const float&  frameVsParticle);
	void setFrameVsParticle(float setFrameVsParticle);

protected:
	RENDER::RParticle* _rParticle;
	std::shared_ptr<RENDER::RParticle::DrawElement> _rElemInParticle;

private:
	bool _bShotKeyDown;

	// Entity을(를) 통해 상속됨
	virtual void setBRender(bool bRender) override;
	virtual void setCollisionTest(bool bCollision) override;
	virtual void doJobWithBeDeleted() override;
	virtual void logicUpdate(float deltaTime, float acc) override;
};
