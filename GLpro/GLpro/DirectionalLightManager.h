#pragma once

#include "stdafx.h"

class DirectionalLight;
struct ShaderStructDirectionalLight;

// todo : dirty bit 이용하여 갱신할 pos만 선택갱신 (RigidbodyComponent todo 완료후 가능)
class DirectionalLightManager
{
public:
	DirectionalLightManager();
	void	initBuffer();
	void	updateLights();
	void	updateLightsToBufferData();
	void	updateBufferToVGA();

	void	addNewDirectionalLight(glm::vec4& color, const glm::vec3& lightLookVec3);
	void	removeDirectionalLight(DirectionalLight* dLight);

	GLuint	getUBO();
	struct ShaderStructDirectionalLight*	getLightStruct();
private:
	void	removeDirectionalLightWithNoDelete(DirectionalLight* dLight);

public:
	friend class DirectionalLight;

private:
	GLuint	_ubo;
	struct ShaderStructDirectionalLight*	_lightUniformBufferData;

	std::vector<DirectionalLight*>			_directionalLightVec;
};