#pragma once

#include <list>
#include <utility>
#include <memory>

#include "../../RRender.h"

namespace SHADER {
	class ShaderParticle;
	class ShaderObj;
}
namespace RENDER_TARGET {
	namespace PARTICLE {
		class ParticleFObj;
	}
}

namespace CAMERA { class Camera; }

class RigidbodyComponent;
class Camera;
class ParticleCreateInfo;

namespace RENDER
{
	class RParticle : public RRender
	{
	public:
		using TYPE_SHADER = SHADER::ShaderParticle;
		using FObjElem = std::shared_ptr<RENDER_TARGET::PARTICLE::ParticleFObj>;
		using DrawElement = ParticleCreateInfo;
		using DrawElemContainer = std::list<std::shared_ptr<DrawElement>>;

	public:
		RParticle(TYPE_SHADER* shaderObj);
		virtual ~RParticle() {};

		std::shared_ptr<DrawElement> addToDrawList(FObjElem particleFObj, RigidbodyComponent* rigidbodyComponent);

		virtual void updateRRender() override;	//shader target camera update
		virtual void drawRRender(float deltaTime) override;
		void updateTargetCamera(CAMERA::Camera** cam) override;

		void chageShader(TYPE_SHADER* other);

		TYPE_SHADER * getShader() const;

	private:
		void beforeDraw(float deltaTime);

	private:
		TYPE_SHADER* _shaderObj;
		CAMERA::Camera **_targetCamera;

		DrawElemContainer _particleDrawElemContainer;
	};
}