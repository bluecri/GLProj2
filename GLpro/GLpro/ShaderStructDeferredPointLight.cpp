#include "ShaderStructDeferredPointLight.h"
#include "LightWithEntity.h"

ShaderStructDeferredPointLight::ShaderStructDeferredPointLight()
{
	initShaderStruct();
}

void ShaderStructDeferredPointLight::initShaderStruct()
{
	_lightNum = 0;
}

/*
void ShaderStructDeferredPointLight::moveData(int from, int to)
{
	_lightPower[to]			= _lightPower[from];
	_lightColorVec4Vec[to]	= _lightColorVec4Vec[from];
	_lightPosVec3Vec[to]	= _lightPosVec3Vec[from];
}
*/

void ShaderStructDeferredPointLight::loadLightsDataToBuffer(std::vector<std::shared_ptr<LightWithEntity>>& lightsDataVec)
{
	initShaderStruct();

	for (auto elem : lightsDataVec)
	{
		if (_lightNum >= MAX_DFFERED_POINTL_LIGHT_NUM)
		{
			printf_s("[LOG] : DirectionalLightManager::updateLightsToBufferData cannot add more light\n");
			return;
		}

		if (!elem->isLightOn())
		{
			continue;
		}

		_lightColorVec4Vec[_lightNum] = elem->getColor();
		_lightColorVec4Vec[_lightNum][3] = elem->getLightPower();

		const glm::vec3& worldpos = elem->getWorldPos();
		for (int i = 0; i < 3; i++)
		{
			_lightPosVec4Vec[_lightNum][i] = worldpos[i];
		}
		_lightNum++;
	}
}