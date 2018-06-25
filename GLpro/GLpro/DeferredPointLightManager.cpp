#include "DeferredPointLightManager.h"

#include "ShaderStructDeferredPointLight.h"
#include "configs_ubo.h"
#include "PointLIght.h"

/*
#include "DeferredPointLightManager.h"
#include "DeferredPointLight.h"
#include "ShaderStructDeferredPointLight.h"

DeferredPointLightManager::DeferredPointLightManager()
{
	_lightUniformBufferData = new ShaderStructDeferredPointLight();
}

void DeferredPointLightManager::initBuffer()
{
	glGenBuffers(1, &_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(_lightUniformBufferData), _lightUniformBufferData, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, UBO_INDEX_DEFERRED_POINT_LIGHT, _ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void DeferredPointLightManager::updateLightsToBufferData()
{
	_lightUniformBufferData->loadLightsDataToBuffer(_defferedPointLightSharedVec);
}

void DeferredPointLightManager::updateBufferToVGA()
{
	glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(_lightUniformBufferData), _lightUniformBufferData);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

std::shared_ptr<DeferredPointLight> DeferredPointLightManager::addNewDeferredPointLight(GameSession* gSession, glm::vec3& pos, glm::vec4 & color, float lightPower)
{
	int newIdx = _defferedPointLightSharedVec.size();
	auto newSharedLight = std::shared_ptr<DeferredPointLight>(new DeferredPointLight(this, gSession, newIdx, pos, color,lightPower));
	newSharedLight->lightInit();

	_defferedPointLightSharedVec.push_back(newSharedLight);
}

void DeferredPointLightManager::removeDeferredPointLight(std::shared_ptr<DeferredPointLight> dLight)
{
	int delIdx = dLight->_idx;
	int lastIdx = _defferedPointLightSharedVec.size();

	// Move last light data to delete light Index
	auto lastLight = _defferedPointLightSharedVec[lastIdx];
	lastLight->_idx = delIdx;

	_defferedPointLightSharedVec[delIdx] = _defferedPointLightSharedVec[lastIdx];

	// delete target Light
	dLight->_isValid = false;
	dLight->_bindedLightGameObject = nullptr;
	_defferedPointLightSharedVec.pop_back();
}

GLuint DeferredPointLightManager::getUBO()
{
	return _ubo;
}
*/

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
	int newIdx = _lightSharedVec.size();
	std::shared_ptr<LightWithEntity> newSharedLight = std::make_shared<PointLight>(this, gSession, UBO_INDEX_POINT_LIGHT, newIdx, pos, color, lightPower);
	newSharedLight->lightInit();

	_lightSharedVec.push_back(newSharedLight);
	return newSharedLight;
}

ShaderStructDeferredPointLight * DeferredPointLightManager::getLightStruct()
{
	return _lightUniformBufferData;
}
