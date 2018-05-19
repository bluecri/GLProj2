#pragma once

#include <list>
#include <utility>
#include <memory>

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
	class RParticle
	{
	public:
		using TYPE_SHADER = SHADER::ShaderParticle;
		using FObjElem = RENDER_TARGET::PARTICLE::ParticleFObj;
		using DrawElement = std::pair<RENDER_TARGET::PARTICLE::ParticleFObj*, ParticleCreateInfo*>;
		using DrawElemContainer = std::list<std::shared_ptr<DrawElement>>;

	public:
		RParticle(TYPE_SHADER* shaderObj);
		virtual ~RParticle() {};

		// use this with weak_ptr
		std::shared_ptr<DrawElement> addToDrawList(FObjElem* particleFObj, RigidbodyComponent* rigidbodyComponent);

		void update(CAMERA::Camera* cam);	//shader target camera update

		void draw(float deltaTime);

		virtual void chageShader(TYPE_SHADER* other);

		virtual TYPE_SHADER * getShader() const;

		void destructor(std::shared_ptr<DrawElement> shared);

	private:
		void beforeDraw(float deltaTime);

	private:
		TYPE_SHADER* _shaderObj;
		CAMERA::Camera *_targetCamera;

		DrawElemContainer _particleDrawElemContainer;
	};
}