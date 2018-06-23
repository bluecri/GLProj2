#pragma once
#include "ILightWithEntityManager.h"

class ShaderStructSpotLight;

class SpotLightManager : public ILightWithEntityManager
{
public:
	SpotLightManager();
	struct ShaderStructSpotLight*		_lightUniformBufferData;

	// ILightWithEntityManager��(��) ���� ��ӵ�
	virtual void initBuffer() override;
	virtual void updateLightsToBufferData() override;
	virtual void updateBufferToVGA() override;

	virtual std::shared_ptr<LightWithEntity> addNewLight(GameSession * gSession, glm::vec3 & dir, glm::vec3 & pos, glm::vec4 & color, float lightPower) override;
};