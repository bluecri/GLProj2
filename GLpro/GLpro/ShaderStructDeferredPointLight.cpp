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

	int cnt = 0;
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

		_lightNum++;
		_lightPower[cnt]		= elem->getLightPower();
		_lightColorVec4Vec[cnt] = elem->getColor();
		_lightPosVec3Vec[cnt]	= elem->getWorldPos();
	}
}