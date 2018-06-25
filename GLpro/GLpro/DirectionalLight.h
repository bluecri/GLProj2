#pragma once

#include "light.h"

class DirectionalLightManager;

// Can delete self or through manager
class DirectionalLight	:	public Light
{
public:
	mat4& GetProjMat() { return _projectionMatrix; }
	mat4& GetViewMat() { return _viewMatrix; }

	mat4& GetModelMat() { return _modelMatrix; }
	vec3& getLightDirVec() { return _lightLookVec3; }
	virtual ~DirectionalLight();
private:
	DirectionalLight(DirectionalLightManager* bindedLightManager, int idx, glm::vec4& color, const glm::vec3& lightLookVec3);

private:
	DirectionalLightManager*	_bindedLightManager;
	int							_idx;

	mat4 _projectionMatrix;
	mat4 _viewMatrix;

	vec3 _lightLookVec3;
	mat4 _modelMatrix;
public:
	friend class DirectionalLightManager;
};