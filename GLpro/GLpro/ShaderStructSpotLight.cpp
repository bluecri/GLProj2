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


		//_lightPower[_lightNum]		= lit->getLightPower();
		_lightColorVec4Vec[_lightNum] = lit->getColor();
		_lightColorVec4Vec[_lightNum][3] = lit->getLightPower();
		_lightPosVec3Vec[_lightNum]	= glm::vec4(lit->getWorldPos(), 0.0f);
		_lightV[_lightNum]			= lit->GetViewMat();
		_lightP[_lightNum]			= lit->GetProjMat();
		_lightNum++;
	}
}
