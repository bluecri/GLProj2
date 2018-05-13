#include "stdafx.h"
#include "LightManager.h"

void LightManager::AddDirectinalLight(DirectionalLight & dLight)
{
	directionalLightVec.push_back(dLight);
}

LightManager* GLightManager = nullptr;