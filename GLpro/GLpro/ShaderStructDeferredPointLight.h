#pragma once

#include "stdafx.h"
#include "configs_light.h"

class LightWithEntity;

struct ShaderStructDeferredPointLight
{
	ShaderStructDeferredPointLight();

	void initShaderStruct();
	//void moveData(int from, int to);
	void loadLightsDataToBuffer(std::vector<std::shared_ptr<LightWithEntity>>& lightsDataVec);

	int			_lightNum;
	glm::vec3	_padding;
	glm::vec4	_lightColorVec4Vec[MAX_DFFERED_POINTL_LIGHT_NUM];	// 3 color 1 light power
	glm::vec4	_lightPosVec4Vec[MAX_DFFERED_POINTL_LIGHT_NUM];
};