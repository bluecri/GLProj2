#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

#include <map>
#include <string>

namespace RESOURCE { class Texture; }
using TextureElemContainer = std::map<std::string, RESOURCE::Texture*>;

namespace RESOURCE
{
	class TextureManager
	{
	public:
		TextureManager() {};
		virtual ~TextureManager();
		Texture* getTextureWithFileName(const std::string &textureFileName, const std::string &type);
		
	private:
		TextureElemContainer _mapTexturesWithFileName;
	};
}

extern RESOURCE::TextureManager* GTextureManager;

#endif