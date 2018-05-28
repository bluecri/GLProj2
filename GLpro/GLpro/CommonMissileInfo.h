#pragma once

#include "stdafx.h"

class CommonMissileInfo
{
public:
	std::string _modelFileName;
	std::string _textureFileName;
	std::string _textureTypeName;
	//ENUM_SHADER_TYPE _shaderType;
	int _shaderType;
	std::string _shaderVertexFileName;
	std::string _shaderFragmentFileName;

	std::string _startSound;
	std::string _hitSound;

	glm::vec3	_genPosAdd;

	float _lifeTime;
};