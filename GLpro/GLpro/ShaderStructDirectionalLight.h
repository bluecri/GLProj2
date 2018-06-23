#pragma once

#include "stdafx.h"
#include "configs_light.h"

class DirectionalLight;

struct ShaderStructDirectionalLight
{
	ShaderStructDirectionalLight();

	void initShaderStruct();
	//void moveData(int from, int to);
	void loadLightsDataToBuffer(std::vector<DirectionalLight*>& lightsDataVec);

	int			_lightNum;
	glm::vec4	_lightColorVec4Vec[MAX_DIRECTIONAL_LIGHT_NUM];
	glm::vec3	_lightLookVec3Vec[MAX_DIRECTIONAL_LIGHT_NUM];
	glm::mat4	_biasedModelMatrixVec[MAX_DIRECTIONAL_LIGHT_NUM];
};
