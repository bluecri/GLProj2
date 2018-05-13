#ifndef __R_SKYBOX_H__
#define __R_SKYBOX_H__

#include "../../stdafx.h"

namespace SHADER { class ShaderSkybox; }
namespace RENDER_TARGET { namespace SKYBOX { class SkyboxFObj; } }

class RigidbodyComponent;
class Camera;

using RSkyboxElement	= std::pair<RENDER_TARGET::SKYBOX::SkyboxFObj*, RigidbodyComponent*>;
using RSkyboxContainer	= std::list<RSkyboxElement>;

namespace RENDER 
{
	class RSkybox
	{
	public:
		RSkybox(SHADER::ShaderSkybox* shaderObj, float div = 1.1f);
		virtual ~RSkybox() {};

		RSkyboxElement* addToDrawList(RENDER_TARGET::SKYBOX::SkyboxFObj* skyboxFObj, RigidbodyComponent* rigidComponent);

		void update(CAMERA::Camera* cam);	//shader target camera update

		void draw();

		virtual void chageShader(SHADER::ShaderSkybox* other);

		virtual SHADER::ShaderObj* getShader() const;

	private:
		SHADER::ShaderSkybox* _shaderObj;
		CAMERA::Camera *_targetCamera;

		float _div = 1.1f;

		RSkyboxContainer _skyboxFobjcetContainer;
	};
}
#endif