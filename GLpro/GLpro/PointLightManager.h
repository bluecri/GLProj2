#pragma once
#include "ILightWithEntityManager.h"

struct ShaderStructPointLight;

class PointLightManager : public ILightWithEntityManager
{
public:
	PointLightManager();
	struct ShaderStructPointLight*		_lightUniformBufferData;

	// ILightWithEntityManager을(를) 통해 상속됨
	virtual void initBuffer() override;
	virtual void updateLights() override;
	virtual void updateLightsToBufferData() override;
	virtual void updateBufferToVGA() override;

	std::shared_ptr<LightWithEntity> addNewLight(GameSession * gSession, glm::vec3 & pos, glm::vec4 & color, float lightPower);
	struct ShaderStructPointLight*	getLightStruct();
};