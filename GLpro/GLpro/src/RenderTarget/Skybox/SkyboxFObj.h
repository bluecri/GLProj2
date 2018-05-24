#ifndef __SKYBOX_F_OBJ_H__
#define __SKYBOX_F_OBJ_H__

#include "../../../Fobj.h"

class SkyboxModel;
class SkyboxTexture;

namespace RENDER_TARGET
{
	namespace SKYBOX
	{
		class SkyboxModel;
		class SkyboxTexture;
	}
}

namespace RENDER_TARGET
{
	namespace SKYBOX
	{
		class SkyboxFObj : public FObj
		{
		public:
			SkyboxFObj(const char* up_path, const char* down_path, const char* left_path, const char* right_path, const char* front_path, const char* back_path, float dist = 40.0f);
			~SkyboxFObj();;

		public:
			SkyboxModel * _skbModel = nullptr;
			SkyboxTexture * _skbTexture = nullptr;

		private:
			SkyboxFObj() {};
		};
	}
}


#endif