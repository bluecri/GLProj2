#include "DeferredPointLightManager.h"

#include "ShaderStructDeferredPointLight.h"
#include "DeferredPointLight.h"
#include "configs_ubo.h"

DeferredPointLightManager::DeferredPointLightManager()
{
	_lightUniformBufferData = new ShaderStructDeferredPointLight();
}

void DeferredPointLightManager::initBuffer()
{
	glGenBuffers(1, &_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(struct ShaderStructDeferredPointLight), _lightUniformBufferData, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, UBO_INDEX_DEFERRED_POINT_LIGHT, _ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void DeferredPointLightManager::updateLights()
{
	for (auto elem : _lightSharedVec)
	{
		elem->lightUpdate();
	}
}

void DeferredPointLightManager::updateLightsToBufferData()
{
	_lightUniformBufferData->loadLightsDataToBuffer(_lightSharedVec);
}

void DeferredPointLightManager::updateBufferToVGA()
{
	glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(struct ShaderStructDeferredPointLight), _lightUniformBufferData);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

std::shared_ptr<LightWithEntity> DeferredPointLightManager::addNewLight(GameSession * gSession, glm::vec3 & pos, glm::vec4 & color, float lightPower)
{
	int newIdx = static_cast<int>(_lightSharedVec.size());
	std::shared_ptr<LightWithEntity> newSharedLight = std::make_shared<DeferredPointLight>(this, gSession, UBO_INDEX_DEFERRED_POINT_LIGHT, newIdx, pos, color, lightPower);
	newSharedLight->lightInit();

	_lightSharedVec.push_back(newSharedLight);
	return newSharedLight;
}

ShaderStructDeferredPointLight * DeferredPointLightManager::getLightStruct()
{
	return _lightUniformBufferData;
}
