#ifndef __R_SKYBOX_H__
#define __R_SKYBOX_H__

#include <list>
#include <utility>
#include <memory>

namespace SHADER {
	class ShaderSkybox; 
	class ShaderObj;
}
namespace RENDER_TARGET { namespace SKYBOX { class SkyboxFObj; } }
namespace CAMERA { class Camera; }

class RigidbodyComponent;
class Camera;

using RSkyboxDrawElement	= std::pair<RENDER_TARGET::SKYBOX::SkyboxFObj*, RigidbodyComponent*>;
using RSkyboxDrawElemContainer	= std::list<std::shared_ptr<RSkyboxDrawElement>>;

namespace RENDER 
{
	class RSkybox
	{
	public:
		RSkybox(SHADER::ShaderSkybox* shaderObj, float div = 1.1f);
		virtual ~RSkybox() {};

		std::shared_ptr<RSkyboxDrawElement> addToDrawList(RENDER_TARGET::SKYBOX::SkyboxFObj* skyboxFObj, RigidbodyComponent* rigidComponent);

		void update(CAMERA::Camera* cam);	//shader target camera update

		void draw();

		virtual void chageShader(SHADER::ShaderSkybox* other);

		virtual SHADER::ShaderObj* getShader() const;

		void destructor(std::shared_ptr<RSkyboxDrawElement> shared);
	private:
		SHADER::ShaderSkybox* _shaderObj;
		CAMERA::Camera *_targetCamera;

		float _div = 1.1f;

		RSkyboxDrawElemContainer _skyboxDrawElemContainer;
	};
}
#endif