#include "ShaderStructPointLight.h"
#include "PointLIght.h"

ShaderStructPointLight::ShaderStructPointLight()
{
	initShaderStruct();
}

void ShaderStructPointLight::initShaderStruct()
{
	_lightNum = 0;
}

/*
void ShaderStructPointLight::moveData(int from, int to)
{
	_lightPower[to]			= _lightPower[from];
	_lightColorVec4Vec[to]	= _lightColorVec4Vec[from];
	_lightPosVec3Vec[to]	= _lightPosVec3Vec[from];
}
*/

void ShaderStructPointLight::loadLightsDataToBuffer(std::vector<std::shared_ptr<LightWithEntity>>& lightsDataVec)
{
	initShaderStruct();

	int cnt = 0;
	for (auto elem : lightsDataVec)
	{
		PointLight* lit = static_cast<PointLight*>(elem.get());
		if (_lightNum >= MAX_POINTL_LIGHT_NUM)
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
		_lightPosVec3Vec[cnt]	= lit->getWorldPos();
		_lightColorVec4Vec[cnt] = lit->getColor();
		for(int i=0; i<6; i++)
			_lightVPMat[cnt][i]		= lit->getVPMatCubeVec().at(i);
	}
}