#pragma once

#include "light.h"
#include "FrustumOb.h"

class DirectionalLightManager;
class RigidbodyComponent;

namespace RENDER_TARGET
{
	namespace NORMAL {
		class NormalFObj;
	}
}

// Can delete self or through manager
class DirectionalLight	:	public Light
{
public:
	using DrawElement = std::pair<RENDER_TARGET::NORMAL::NormalFObj*, RigidbodyComponent*>;
	using DrawElemContainer = std::list<std::shared_ptr<DrawElement>>;
	virtual ~DirectionalLight();

	void lightUpdate();

	mat4& GetProjMat() { return _projectionMatrix; }
	mat4& GetViewMat() { return _viewMatrix; }

	//mat4& GetModelMat() { return _modelMatrix; }
	vec3& getLightDirVec() { return _lightLookVec3; }

	FrustumOb& getFrustumObRef();

	DrawElemContainer&			getFrustumedDrawElementContainerRef();
	const DrawElemContainer&	getConstFrustumedDrawElementContainerRef();
	void						clearFrustumedDrawElementContainer();
private:
	DirectionalLight(DirectionalLightManager* bindedLightManager, int idx, glm::vec4& color, const glm::vec3& lightLookVec3);

private:
	DirectionalLightManager*	_bindedLightManager;
	int							_idx;

	mat4 _projectionMatrix;
	mat4 _viewMatrix;

	vec3 _lightLookVec3;
	//mat4 _modelMatrix;

	FrustumOb	_frustumOb;
	DrawElemContainer _frustumedDrawElementContainer;

public:
	friend class DirectionalLightManager;
};