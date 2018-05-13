#pragma once

#include "stdafx.h"

class DirectionalLight
{
public:
	DirectionalLight(const glm::vec3& lightLookVec3)
		: _lightLookVec3(lightLookVec3), _depthModelMatrix(1.0f)
	{
		//_depthModelMatrix = glm::mat4(1.0);
		_depthProjectionMatrix	=	ortho<float>(-10, 10, -10, 10, -10, 20);
		_depthViewMatrix	=	lookAt(lightLookVec3, vec3(0, 0, 0), vec3(0, 1, 0));
		_biasMatrix	= mat4(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
		);

		// create proxy matrix
		_depthBiasMVP = _biasMatrix * _depthProjectionMatrix * _depthViewMatrix * _depthModelMatrix;
		_depthMVPUpper = _depthProjectionMatrix * _depthViewMatrix * _depthModelMatrix;
	}

	mat4& GetDepthBiasMVP() { return _depthBiasMVP; }
	mat4& GetDepthMVPUpper() { return _depthMVPUpper; }
	vec3& GetLightVec3() { return _lightLookVec3; }

private:
	mat4 _depthProjectionMatrix;
	mat4 _depthViewMatrix;
	mat4 _depthModelMatrix;
	mat4 _biasMatrix;

	vec3 _lightLookVec3;
	mat4 _depthBiasMVP;
	mat4 _depthMVPUpper;
};