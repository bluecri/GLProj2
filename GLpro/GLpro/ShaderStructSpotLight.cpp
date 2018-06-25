#include "ShaderStructSpotLight.h"
#include "SpotLIght.h"

ShaderStructSpotLight::ShaderStructSpotLight()
{
	initShaderStruct();
}

void ShaderStructSpotLight::initShaderStruct()
{
	_lightNum = 0;
}
/*
void ShaderStructSpotLight::moveData(int from, int to)
{
	_lightPower[to]				= _lightPower[from];
	_lightColorVec4Vec[to]		= _lightColorVec4Vec[from];
	_lightPosVec3Vec[to]		= _lightPosVec3Vec[from];
}
*/

void ShaderStructSpotLight::loadLightsDataToBuffer(std::vector<std::shared_ptr<LightWithEntity>>& lightsDataVec)
{
	initShaderStruct();

	int cnt = 0;
	for (auto elem : lightsDataVec)
	{
		SpotLight* lit = static_cast<SpotLight*>(elem.get());
		if (_lightNum >= MAX_SPOT_LIGHT_NUM)
		{
			printf_s("[LOG] : DirectionalLightManager::updateLightsToBufferData cannot add more light\n");
			return;
		}

		if (!lit->isLightOn())
		{
			continue;
		}

		_lightNum++;

		_lightPower[cnt]		= lit->getLightPower();
		_lightColorVec4Vec[cnt] = lit->getColor();
		_lightPosVec3Vec[cnt]	= lit->getWorldPos();
		_lightV[cnt]			= lit->GetViewMat();
		_lightP[cnt]			= lit->GetProjMat();
	}
}
