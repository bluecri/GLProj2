#ifndef __R_NORMAL_H__
#define __R_NORMAL_H__

#include <list>
#include <utility>
#include <memory>

#include "../../RRender.h"

namespace CAMERA { class Camera; }

namespace RENDER_TARGET{ namespace NORMAL { class NormalFObj; } }
namespace SHADER {
	class ShaderMain; class ShaderShadow;
}

class RigidbodyComponent;


namespace RENDER
{
	class RNormal : public RRender
	{
		public:
			using TYPE_SHADER = SHADER::ShaderMain;
			using DrawElement = std::pair<RENDER_TARGET::NORMAL::NormalFObj*, RigidbodyComponent*>;
			using SharedDrawElement = std::shared_ptr<DrawElement>;
			using DrawElemContainer = std::list<SharedDrawElement>;

			RNormal(SHADER::ShaderMain* shaderObj);
			virtual ~RNormal() {};	// Render 삭제시 DrawElemContainer이 shared_ptr 이므로 자동 해제.

			std::shared_ptr<RNormal::DrawElement> addDrawElem(RENDER_TARGET::NORMAL::NormalFObj * normalFObj, RigidbodyComponent* rigidComponent);
			DrawElemContainer& getDrawElemList();

			void updateRRender() override;	//shader target camera update
			void updateTargetCamera(CAMERA::Camera** cam) override;

			void shadowBufferPreDraw(float deltaTime);
			void shadowBufferDraw(float deltaTime);
			void shadowMappingDraw(float deltaTime);

			void deferredDraw(float deltaTime);


			virtual void drawRRender(float deltaTime);

			//	virtual void chageShader(SHADER::ShaderMain* other);	-> make fixed.

			SHADER::ShaderMain* getShader() const;

	private:
			SHADER::ShaderMain* _shaderObj;
			CAMERA::Camera **_targetCamera;

			SHADER::ShaderShadow* _oldShaderShadow;

			DrawElemContainer _normalDrawElemContainer;
	};
}
#endif