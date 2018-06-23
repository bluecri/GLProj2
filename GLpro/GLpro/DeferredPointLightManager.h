#include "ILightWithEntityManager.h"

class ShaderStructDeferredPointLight;

class DeferredPointLightManager : public ILightWithEntityManager
{
public :
	DeferredPointLightManager();
	struct ShaderStructDeferredPointLight*		_lightUniformBufferData;

	// ILightWithEntityManager을(를) 통해 상속됨
	virtual void initBuffer() override;
	virtual void updateLightsToBufferData() override;
	virtual void updateBufferToVGA() override;
	virtual std::shared_ptr<LightWithEntity> addNewLight(GameSession * gSession, glm::vec3 & dir, glm::vec3 & pos, glm::vec4 & color, float lightPower) override;

};