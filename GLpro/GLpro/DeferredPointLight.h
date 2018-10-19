#pragma once

#include "LightWithEntity.h"

class DeferredPointLight : public LightWithEntity
{
public:
	DeferredPointLight(ILightWithEntityManager* bindedLightManager, GameSession* gSession, int uboType, int idx, glm::vec3& pos, glm::vec4& color, float lightPower);

	// LightWithEntity을(를) 통해 상속됨
	virtual void lightUpdate() override;

};