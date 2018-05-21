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

namespace RENDER 
{
	class RSkybox
	{
	public:
		using TYPE_SHADER = SHADER::ShaderSkybox;
		using DrawElement = std::pair<RENDER_TARGET::SKYBOX::SkyboxFObj*, RigidbodyComponent*>;
		using DrawElemContainer = std::list<std::shared_ptr<DrawElement>>;
	public:
		RSkybox(SHADER::ShaderSkybox* shaderObj, float div = 1.1f);
		virtual ~RSkybox() {};

		std::shared_ptr<DrawElement> addToDrawList(RENDER_TARGET::SKYBOX::SkyboxFObj* skyboxFObj, RigidbodyComponent* rigidComponent);

		void update(CAMERA::Camera* cam);	//shader target camera update

		void draw();

		virtual void chageShader(SHADER::ShaderSkybox* other);

		virtual SHADER::ShaderObj* getShader() const;

		void destructor(std::shared_ptr<DrawElement> shared);
	private:
		SHADER::ShaderSkybox* _shaderObj;
		CAMERA::Camera *_targetCamera;

		float _div = 1.1f;

		DrawElemContainer _skyboxDrawElemContainer;
	};
}
#endif