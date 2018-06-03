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
		using FObjElem = RENDER_TARGET::PARTICLE::ParticleFObj;
		using DrawElement = std::pair<RENDER_TARGET::PARTICLE::ParticleFObj*, ParticleCreateInfo*>;
		using DrawElemContainer = std::list<std::shared_ptr<DrawElement>>;

	public:
		RParticle(TYPE_SHADER* shaderObj);
		virtual ~RParticle() {};

		std::shared_ptr<DrawElement> addToDrawList(FObjElem* particleFObj, RigidbodyComponent* rigidbodyComponent);

		virtual void update(CAMERA::Camera** cam);	//shader target camera update

		virtual void draw(float deltaTime);

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