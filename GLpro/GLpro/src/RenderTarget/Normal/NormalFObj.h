#ifndef __Normal_F_OBJ_H__
#define __Normal_F_OBJ_H__

#include "../../../Fobj.h"

namespace RESOURCE
{
	class Model;
	class Texture;
}

namespace RENDER_TARGET
{
	namespace NORMAL
	{
		class NormalFObj : public FObj
		{
		public:
			NormalFObj(const char* modelFileName, bool createEbo, const char* textureFileName, const char* textureType);
			NormalFObj(RESOURCE::Model * model, RESOURCE::Texture * texture);
			~NormalFObj() {	};

		public:
			RESOURCE::Model * _model = nullptr;
			RESOURCE::Texture * _texture = nullptr;

		private:
			NormalFObj() {};
		};
	}
}


#endif