#include "TextureManager.h"

#include "./Texture.h"

namespace RESOURCE
{
	TextureManager::~TextureManager()
	{
		std::for_each(_mapTexturesWithFileName.begin(), _mapTexturesWithFileName.end(), [](auto &elem)
		{
			delete elem.second;
		});
	}

	Texture * TextureManager::getTextureWithFileName(const std::string & textureFileName, const std::string & type)
	{
		// Find texture already loaded
		auto mapTextureName = _mapTexturesWithFileName.find(textureFileName);

		if (mapTextureName != _mapTexturesWithFileName.end())
		{
			return mapTextureName->second;	// Return preLoaded Texture
		}

		// Load new texture from file
		Texture* makeNewTexture = new Texture(textureFileName, type);
		_mapTexturesWithFileName.insert(std::make_pair(textureFileName, makeNewTexture));

		return makeNewTexture;
	}
}

RESOURCE::TextureManager* GTextureManager = nullptr;
