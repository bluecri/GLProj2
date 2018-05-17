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

using RParticleDrawElement = std::pair<RENDER_TARGET::PARTICLE::ParticleFObj*, ParticleCreateInfo*>;
using RParticleDrawElemContainer = std::list<std::shared_ptr<RParticleDrawElement>>;

namespace RENDER
{
	class RParticle
	{
	public:
		RParticle(SHADER::ShaderParticle* shaderObj);
		virtual ~RParticle() {};

		// use this with weak_ptr
		std::shared_ptr<RParticleDrawElement> addToDrawList(RENDER_TARGET::PARTICLE::ParticleFObj* particleFObj, RigidbodyComponent* rigidbodyComponent);

		void update(CAMERA::Camera* cam);	//shader target camera update

		void draw(float deltaTime);

		virtual void chageShader(SHADER::ShaderParticle* other);

		virtual SHADER::ShaderParticle* getShader() const;

		void destructor(std::shared_ptr<RParticleDrawElement> shared);

	private:
		void beforeDraw(float deltaTime);

	private:
		SHADER::ShaderParticle* _shaderObj;
		CAMERA::Camera *_targetCamera;

		RParticleDrawElemContainer _particleDrawElemContainer;
	};
}