#pragma once

#include "stdafx.h"
#include "DirectionalLight.h"

class LightManager
{
public:
	LightManager() {};
	~LightManager() {};

	void AddDirectinalLight(DirectionalLight& dLight);

public:
	std::vector<DirectionalLight> directionalLightVec;

};

extern LightManager* GLightManager;