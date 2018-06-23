#include "ShaderStructSpotLight.h"
#include "LightWithEntity.h"

ShaderStructSpotLight::ShaderStructSpotLight()
{
	initShaderStruct();
}

void ShaderStructSpotLight::initShaderStruct()
{
	_lightNum = 0;
}

void ShaderStructSpotLight::moveData(int from, int to)
{
	_lightPower[to]				= _lightPower[from];
	_lightColorVec4Vec[to]		= _lightColorVec4Vec[from];
	_lightPosVec3Vec[to]		= _lightPosVec3Vec[from];
	_lightLookVec3Vec[to]		= _lightLookVec3Vec[from];
	_biasMatrixVec[to]	= _biasMatrixVec[from];
}

void ShaderStructSpotLight::loadLightsDataToBuffer(std::vector<std::shared_ptr<LightWithEntity>>& lightsDataVec)
{
	initShaderStruct();

	int cnt = 0;
	for (auto elem : lightsDataVec)
	{
		if (_lightNum >= MAX_SPOT_LIGHT_NUM)
		{
			printf_s("[LOG] : DirectionalLightManager::updateLightsToBufferData cannot add more light\n");
			return;
		}

		if (!elem->isLightOn())
		{
			continue;
		}

		_lightNum++;

		_lightPower[cnt] = elem->getLightPower();
		_lightColorVec4Vec[cnt] = elem->getColor();
		_lightPosVec3Vec[cnt] = elem->getWorldPos();
		_lightLookVec3Vec[cnt] = elem->getLightDir();
		_biasMatrixVec[cnt] = elem->getBiasMat();
	}
}
