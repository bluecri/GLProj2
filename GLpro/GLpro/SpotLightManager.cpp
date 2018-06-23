#include "SpotLightManager.h"

#include "ShaderStructSpotLight.h"
#include "configs_ubo.h"
#include "LightWithEntity.h"

SpotLightManager::SpotLightManager()
{
	_lightUniformBufferData = new ShaderStructSpotLight();
}

void SpotLightManager::initBuffer()
{
	glGenBuffers(1, &_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(_lightUniformBufferData), _lightUniformBufferData, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, UBO_INDEX_SPOT_LIGHT, _ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SpotLightManager::updateLightsToBufferData()
{
	_lightUniformBufferData->loadLightsDataToBuffer(_lightSharedVec);
}

void SpotLightManager::updateBufferToVGA()
{
	glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(_lightUniformBufferData), _lightUniformBufferData);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

std::shared_ptr<LightWithEntity> SpotLightManager::addNewLight(GameSession * gSession, glm::vec3 & dir, glm::vec3 & pos, glm::vec4 & color, float lightPower)
{
	int newIdx = _lightSharedVec.size();
	auto newSharedLight = std::shared_ptr<LightWithEntity>(new LightWithEntity(this, gSession, UBO_INDEX_SPOT_LIGHT, newIdx, dir, pos, color, lightPower));
	newSharedLight->lightInit();

	_lightSharedVec.push_back(newSharedLight);
	return newSharedLight;
}
