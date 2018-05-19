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


namespace RENDER
{
	class RNormal
	{
		public:
			using TYPE_SHADER = SHADER::ShaderMain;
			using DrawElement = std::pair<RENDER_TARGET::NORMAL::NormalFObj*, RigidbodyComponent*>;
			using DrawElemContainer = std::list<std::shared_ptr<DrawElement>>;

			RNormal(SHADER::ShaderMain* shaderObj);
			virtual ~RNormal() {};	// Render 삭제시 DrawElemContainer이 shared_ptr 이므로 자동 해제.

			std::shared_ptr<RNormal::DrawElement> addDrawElem(RENDER_TARGET::NORMAL::NormalFObj * normalFObj, RigidbodyComponent* rigidComponent);

			void update(CAMERA::Camera* cam);	//shader target camera update

			void draw();

			//	virtual void chageShader(SHADER::ShaderMain* other);	-> make fixed.

			virtual SHADER::ShaderMain* getShader() const;

			void removeDrawElem(std::shared_ptr<DrawElement> shared);		
	private:
			SHADER::ShaderMain* _shaderObj;
			CAMERA::Camera *_targetCamera;

			SHADER::ShaderShadow* _oldShaderShadow;

			DrawElemContainer _normalDrawElemContainer;
	};
}
#endif