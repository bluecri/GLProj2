#ifndef __SKYBOX_TEXTURE_GROUP_H__
#define __SKYBOX_TEXTURE_GROUP_H__

#include "../../Resource/Texture.h"
#include "../../3rdParty/TextureLoader/textureLoader.h"

/*
	skybox texture group : has 6 texture.
*/
namespace RENDER_TARGET
{
	namespace SKYBOX
	{
		class SkyboxTexture
		{
		public:
			enum ENUM_TEXTURE_IDX {
				UP = 0, DOWN, LEFT, RIGHT, FRONT, BACK, SIZE_ENUM_TEXTURE_IDX
			};

			SkyboxTexture(const char* up_path, const char* down_path, const char* left_path, const char* right_path, const char* front_path, const char* back_path)
			{
				textureID[UP] = new RESOURCE::Texture(up_path, "DDS");
				textureID[DOWN] = new RESOURCE::Texture(down_path, "DDS");
				textureID[LEFT] = new RESOURCE::Texture(left_path, "DDS");
				textureID[RIGHT] = new RESOURCE::Texture(right_path, "DDS");
				textureID[FRONT] = new RESOURCE::Texture(front_path, "DDS");
				textureID[BACK] = new RESOURCE::Texture(back_path, "DDS");

				for (int i = 0; i < 6; i++) {
					textureID[i]->bind();
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					textureID[i]->unbind();
				}

			}

			~SkyboxTexture() {
				for (int i = 0; i < 6; i++) {
					delete textureID[i];
				}
			}
			void bind(int idx) {
				textureID[idx]->bind();
			}

			void unbind(int idx) {
				textureID[idx]->unbind();
			}
		private:
			RESOURCE::Texture* textureID[6];
		};
	}
}

#endif
