#pragma once

#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "../../Resource/Texture.h"
#include "../../3rdParty/TextureLoader/textureLoader.h"

#include "./SkyboxModel.h"
#include "./SkyboxTexture.h"

#include "../../Resource/ModelWithNoIndiceNoFile.h"
namespace RENDER_TARGET
{
	namespace SKYBOX
	{
		class SkyboxRenderTarget {
		public:
			SkyboxModel * skbModel;
			SkyboxTexture * skbTexture;

			SkyboxRenderTarget(const char* up_path, const char* down_path, const char* left_path, const char* right_path, const char* front_path, const char* back_path, float dist = 40.0f) {
				skbTexture = new SkyboxTexture(up_path, down_path, left_path, right_path, front_path, back_path);
				skbModel = new SkyboxModel(40.0f);
			}
			~SkyboxRenderTarget() {
				delete skbModel;
				delete skbTexture;
			}

		private:
			SkyboxRenderTarget() {};
		};
	}
}

