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

		glm::vec3 worldPos = lit->getWorldPos();
		for (int i = 0; i < 3; i++)
		{
			_lightPosVec3Vec[_lightNum][i] = worldPos[i];
		}
		_lightPosVec3Vec[_lightNum][3] = 0.0f;
		
		_lightColorVec4Vec[_lightNum][3] = lit->getLightPower();
		_lightColorVec4Vec[_lightNum] = lit->getColor();
		for(int i=0; i<6; i++)
			_lightVPMat[_lightNum * 6 + i]		= lit->getVPMatCubeVec().at(i);
		_lightNum++;
	}
}