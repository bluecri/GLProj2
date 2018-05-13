#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

#include "../../stdafx.h"

class Texture;

namespace RESOURCE
{
	class TextureManager
	{
	public:
		//TODO : try to make unique_ptr texture ?
		TextureManager() {};
		virtual ~TextureManager();
		Texture* getTextureWithFileName(const std::string &textureFileName, const std::string &type);
		
	private:
		std::map<std::string, Texture*> _mapTexturesWithFileName;
	};
}

extern RESOURCE::TextureManager* GTextureManager;

#endif