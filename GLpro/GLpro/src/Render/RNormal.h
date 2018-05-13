#ifndef __R_NORMAL_H__
#define __R_NORMAL_H__

#include "../../stdafx.h"

class Camera;

namespace RENDER_TARGET{ namespace NORMAL { class NormalFObj; } }
namespace SHADER { class ShaderMain; }

using NormalFObjContainer = std::vector<RENDER_TARGET::NORMAL::NormalFObj*>;

namespace RENDER
{
	class RNormal
	{
		public:
			RNormal(SHADER::ShaderMain* shaderObj, float div = 1.1f);
			virtual ~RNormal() {};

			void addToDrawList(RENDER_TARGET::NORMAL::NormalFObj* skyboxFObj);

			void update(CAMERA::Camera* cam);	//shader target camera update

			void draw();

			virtual void chageShader(SHADER::ShaderMain* other);

			virtual SHADER::ShaderMain* getShader() const;

		private:
			SHADER::ShaderMain* _shaderObj;
			CAMERA::Camera *_targetCamera;

			NormalFObjContainer _normalFobjcetContainer;
	};
}
#endif