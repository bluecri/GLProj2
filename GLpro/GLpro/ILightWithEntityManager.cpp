#include "ILightWithEntityManager.h"
#include "LightWithEntity.h"

ILightWithEntityManager::ILightWithEntityManager()
{
	//_lightUniformBufferData = new TStruct();
}


void ILightWithEntityManager::removeLightWithEntity(std::shared_ptr<LightWithEntity> dLight)
{
	int delIdx = dLight->_idx;
	int lastIdx = static_cast<int>(_lightSharedVec.size());

	// Move last light data to delete light Index
	auto lastLight = _lightSharedVec[lastIdx];
	lastLight->_idx = delIdx;

	_lightSharedVec[delIdx] = _lightSharedVec[lastIdx];

	// delete target Light
	dLight->_isValid = false;
	dLight->_bindedLightGameObject = nullptr;
	_lightSharedVec.pop_back();
}

GLuint ILightWithEntityManager::getUBO()
{
	return _ubo;
}
