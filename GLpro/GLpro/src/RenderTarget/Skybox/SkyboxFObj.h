#ifndef __SKYBOX_F_OBJ_H__
#define __SKYBOX_F_OBJ_H__

#include "./SkyboxModel.h"
#include "./SkyboxTexture.h"

namespace RENDER_TARGET
{
	namespace SKYBOX
	{
		class SkyboxFObj
		{
		public:
			SkyboxModel * _skbModel = nullptr;
			SkyboxTexture * _skbTexture = nullptr;
			
			SkyboxFObj(const char* up_path, const char* down_path, const char* left_path, const char* right_path, const char* front_path, const char* back_path, float dist = 40.0f) 
			{
				_skbTexture = new SkyboxTexture(up_path, down_path, left_path, right_path, front_path, back_path);
				_skbModel = new SkyboxModel(dist);
			}
			~SkyboxFObj() {	};

		private:
			SkyboxFObj() {};
		};
	}
}


#endif