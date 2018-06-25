#include "ShaderStructDirectionalLight.h"
#include "DirectionalLight.h"

ShaderStructDirectionalLight::ShaderStructDirectionalLight()
{
	initShaderStruct();
}

void ShaderStructDirectionalLight::initShaderStruct()
{
	_lightNum = 0;
}

/*
void ShaderStructDirectionalLight::moveData(int from, int to)
{
	_lightLookVec3Vec[to]		= _lightLookVec3Vec[from];
	_biasedModelMatrixVec[to]	= _biasedModelMatrixVec[from];
	_lightColorVec4Vec[to]		= _lightColorVec4Vec[from];
}
*/

void ShaderStructDirectionalLight::loadLightsDataToBuffer(std::vector<DirectionalLight*>& lightsDataVec)
{
	initShaderStruct();

	int cnt = 0;
	for (auto elem : lightsDataVec)
	{
		if (_lightNum >= MAX_DIRECTIONAL_LIGHT_NUM)
		{
			printf_s("[LOG] : DirectionalLightManager::updateLightsToBufferData cannot add more light\n");
			return;
		}

		if (!elem->isLightOn())
		{
			continue;
		}

		_lightNum++;
		_lightVMat[cnt]				= elem->GetViewMat();
		_lightPMat[cnt]				= elem->GetProjMat();
		_lightColorVec4Vec[cnt]		= elem->getColor();
	}
}
