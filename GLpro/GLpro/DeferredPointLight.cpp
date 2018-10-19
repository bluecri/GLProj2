#include "DeferredPointLight.h"
#include "DeferredPointLightManager.h"
#include "LightGameObject.h"

DeferredPointLight::DeferredPointLight(ILightWithEntityManager * bindedLightManager, GameSession * gSession, int uboType, int idx, glm::vec3 & pos, glm::vec4 & color, float lightPower)
	: LightWithEntity(bindedLightManager, gSession, uboType, idx, glm::mat4(), pos, color, lightPower)
{
}

void DeferredPointLight::lightUpdate()
{
}
