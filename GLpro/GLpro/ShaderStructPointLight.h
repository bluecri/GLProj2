#pragma once

#include "stdafx.h"
#include "configs_light.h"

class LightWithEntity;

struct ShaderStructPointLight
{
	ShaderStructPointLight();

	void initShaderStruct();
	//void moveData(int from, int to);
	void loadLightsDataToBuffer(std::vector<std::shared_ptr<LightWithEntity>>& lightsDataVec);

	int			_lightNum;
	glm::vec3	_padding;
	//float		_lightPower[MAX_POINTL_LIGHT_NUM];
	glm::vec4	_lightColorVec4Vec[MAX_POINTL_LIGHT_NUM];	// + light power
	glm::vec4	_lightPosVec3Vec[MAX_POINTL_LIGHT_NUM];
	glm::mat4	_lightVPMat[MAX_POINTL_LIGHT_NUM * 6];
};