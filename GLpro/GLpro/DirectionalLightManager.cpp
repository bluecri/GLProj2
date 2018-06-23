#include "DirectionalLightManager.h"
#include "DirectionalLight.h"
#include "ShaderStructDirectionalLight.h"
#include "configs_ubo.h"

DirectionalLightManager::DirectionalLightManager() 
{
	_lightUniformBufferData = new ShaderStructDirectionalLight();
}

void DirectionalLightManager::initBuffer()
{
	glGenBuffers(1, &_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(_lightUniformBufferData), _lightUniformBufferData, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, UBO_INDEX_DIRECTIONAL_LIGHT, _ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void DirectionalLightManager::updateLightsToBufferData()
{
	_lightUniformBufferData->loadLightsDataToBuffer(_directionalLightVec);
}

void DirectionalLightManager::updateBufferToVGA()
{
	glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(_lightUniformBufferData), _lightUniformBufferData);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void DirectionalLightManager::addNewDirectionalLight(glm::vec4 & color, const glm::vec3 & lightLookVec3)
{
	
	int newIdx = _directionalLightVec.size();
	DirectionalLight* newLight = new DirectionalLight(this, newIdx, color, lightLookVec3);
	_directionalLightVec.push_back(newLight);
}

void DirectionalLightManager::removeDirectionalLight(DirectionalLight * dLight)
{
	int delIdx = dLight->_idx;
	int lastIdx = _directionalLightVec.size();

	// Move last light data to delete light Index
	DirectionalLight* lastLight = _directionalLightVec[lastIdx];
	lastLight->_idx = delIdx;

	_directionalLightVec[delIdx] = _directionalLightVec[lastIdx];

	// delete target Light
	delete dLight;
	_directionalLightVec.pop_back();
}

GLuint DirectionalLightManager::getUBO()
{
	return _ubo;
}

void DirectionalLightManager::removeDirectionalLightWithNoDelete(DirectionalLight * dLight)
{
	int delIdx = dLight->_idx;
	int lastIdx = _directionalLightVec.size();

	// Move last light data to delete light Index
	DirectionalLight* lastLight = _directionalLightVec[lastIdx];
	lastLight->_idx = delIdx;

	_directionalLightVec[delIdx] = _directionalLightVec[lastIdx];

	// delete target Light
	// delete dLight;
	_directionalLightVec.pop_back();
}
