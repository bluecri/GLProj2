#ifndef __R_NORMAL_H__
#define __R_NORMAL_H__

#include <list>
#include <utility>
#include <memory>

namespace CAMERA { class Camera; }

namespace RENDER_TARGET{ namespace NORMAL { class NormalFObj; } }
namespace SHADER {
	class ShaderMain; class ShaderShadow;
}

class RigidbodyComponent;

using TYPE_SHADER = SHADER::ShaderMain;

using RNormalDrawElement = std::pair<RENDER_TARGET::NORMAL::NormalFObj*, RigidbodyComponent*>;
using RNormalDrawElemContainer = std::list<std::shared_ptr<RNormalDrawElement>>;

namespace RENDER
{
	class RNormal : public RReneder
	{
		public:
			RNormal(SHADER::ShaderMain* shaderObj);
			virtual ~RNormal() {};

			std::shared_ptr<RNormalDrawElement> addToDrawList(RENDER_TARGET::NORMAL::NormalFObj * normalFObj, RigidbodyComponent* rigidComponent);

			void update(CAMERA::Camera* cam);	//shader target camera update

			void draw();

			//	virtual void chageShader(SHADER::ShaderMain* other);	-> make fixed.

			virtual SHADER::ShaderMain* getShader() const;

			void destructor(std::shared_ptr<RNormalDrawElement> shared);
	private:
			SHADER::ShaderMain* _shaderObj;
			CAMERA::Camera *_targetCamera;

			SHADER::ShaderShadow* _oldShaderShadow;

			RNormalDrawElemContainer _normalDrawElemContainer;
	};
}
#endif