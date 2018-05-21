#include "NormalFObj.h"

#include "../../Resource/Texture.h"
#include "../../Resource/Model.h"

#include "../../Resource/ModelManager.h"
#include "../../Resource/TextureManager.h"

namespace RENDER_TARGET
{
	namespace NORMAL
	{
		NormalFObj::NormalFObj(const char * modelFileName, bool createEbo, const char * textureFileName, const char * textureType)
		{
			_model = GModelManager->getModelWithFileName(modelFileName, createEbo);
			_texture = GTextureManager->getTextureWithFileName(textureFileName, textureType);
		}
		NormalFObj::NormalFObj(RESOURCE::Model * model, RESOURCE::Texture * texture)
		{
			_model = model;
			_texture = texture;
		}
	}
}


