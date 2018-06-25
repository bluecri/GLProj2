#pragma once
#include "ILightWithEntityManager.h"

struct ShaderStructSpotLight;

class SpotLightManager : public ILightWithEntityManager
{
public:
	SpotLightManager();
	struct ShaderStructSpotLight*		_lightUniformBufferData;

	// ILightWithEntityManager을(를) 통해 상속됨
	virtual void initBuffer() override;
	virtual void updateLightsToBufferData() override;
	virtual void updateBufferToVGA() override;

	std::shared_ptr<LightWithEntity> addNewLight(GameSession * gSession, glm::mat4 & rotMat, glm::vec3 & pos, glm::vec4 & color, float lightPower, float fovy, float aspcect, float persNear, float persFar);
	struct ShaderStructSpotLight*	getLightStruct();
};