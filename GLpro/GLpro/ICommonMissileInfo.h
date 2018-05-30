#pragma once
#include "stdafx.h"
#include "./src/Shader/ShaderManager.h"


class ICommonMissileInfo
{
public:
	virtual void init() = 0;
	virtual void init(std::string & commonMissileInfoFile) = 0;

public:
	std::string _modelFileName;
	std::string _textureFileName;
	std::string _textureTypeName;
	ENUM_SHADER_TYPE _shaderType;
	//int _shaderType;
	std::string _shaderVertexFileName;
	std::string _shaderFragmentFileName;

	glm::mat4 _collisionBoxMat;
	glm::vec3 _missileCollisionBoxAxis;

	float _shotDelay;
};