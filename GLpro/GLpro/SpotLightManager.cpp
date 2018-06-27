#include "SpotLightManager.h"

#include "ShaderStructSpotLight.h"
#include "configs_ubo.h"
#include "SpotLight.h"

SpotLightManager::SpotLightManager()
{
	_lightUniformBufferData = new ShaderStructSpotLight();
}

void SpotLightManager::initBuffer()
{
	glGenBuffers(1, &_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(struct ShaderStructSpotLight), _lightUniformBufferData, GL_DYNAMIC_DRAW);
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
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(struct ShaderStructSpotLight), _lightUniformBufferData);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

std::shared_ptr<LightWithEntity> SpotLightManager::addNewLight(GameSession * gSession, glm::mat4 & rotMat, glm::vec3 & pos, glm::vec4 & color, float lightPower, float fovy, float aspcect, float persNear, float persFar)
{
	int newIdx = static_cast<int>(_lightSharedVec.size());
	std::shared_ptr<LightWithEntity>newSharedLight = std::make_shared<SpotLight>(this, gSession, UBO_INDEX_SPOT_LIGHT, newIdx, rotMat, pos, color, lightPower, fovy, aspcect, persNear, persFar);
	newSharedLight->lightInit();

	_lightSharedVec.push_back(newSharedLight);
	return newSharedLight;
}

ShaderStructSpotLight * SpotLightManager::getLightStruct()
{
	return _lightUniformBufferData;
}
