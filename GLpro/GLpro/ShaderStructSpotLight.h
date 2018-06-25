#pragma once

#include "stdafx.h"
#include "configs_light.h"

class LightWithEntity;

struct ShaderStructSpotLight
{
	ShaderStructSpotLight();

	void initShaderStruct();
	//void moveData(int from, int to);
	void loadLightsDataToBuffer(std::vector<std::shared_ptr<LightWithEntity>>& lightsDataVec);

	int			_lightNum;
	float		_lightPower[MAX_SPOT_LIGHT_NUM];
	glm::vec4	_lightColorVec4Vec[MAX_SPOT_LIGHT_NUM];
	glm::vec3	_lightPosVec3Vec[MAX_SPOT_LIGHT_NUM];
	glm::mat4	_lightV[MAX_SPOT_LIGHT_NUM];
	glm::mat4	_lightP[MAX_SPOT_LIGHT_NUM];
};
