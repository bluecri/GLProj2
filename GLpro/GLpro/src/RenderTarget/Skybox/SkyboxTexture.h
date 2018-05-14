#ifndef __SKYBOX_TEXTURE_GROUP_H__
#define __SKYBOX_TEXTURE_GROUP_H__

namespace RESOURCE { class Texture; }

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

			SkyboxTexture(const char* up_path, const char* down_path, const char* left_path, const char* right_path, const char* front_path, const char* back_path);
			~SkyboxTexture();
			void bind(int idx);
			void unbind(int idx);

		private:
			RESOURCE::Texture* textureID[6];
		};
	}
}

#endif
