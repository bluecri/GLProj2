#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

#include "GL/glew.h"
#include <glm/gtx/transform.hpp>

#include <vector>
#include <cstring>
#include <iostream>
#include <algorithm>

#include "./Texture.h"

#include "../../3rdParty/TextureLoader/textureLoader.h"
#include "../../3rdParty/objectLoader/Vertex.h"
#include "../../3rdParty/objectLoader/objectLoader.h"
#include "../../3rdParty/objectLoader/vboindexer.h"

namespace RESOURCE
{
	class TextureManager {
		public:
			//TODO : try to make unique_ptr texture
			TextureManager() {};
			virtual ~TextureManager() 
			{
				std::for_each(_mapTexturesWithFileName.begin(), _mapTexturesWithFileName.end(), [](auto &elem)
				{
					delete elem->second;
				});
			};
			Texture* getTextureWithFileName(const std::string &textureFileName, const std::string &type)
			{
				// Find texture already loaded
				auto mapTextureName = _mapTexturesWithFileName.find(std::string(textureFileName));

				if (mapTextureName != _mapTexturesWithFileName.end())
				{
					return mapTextureName->second;	// Return preLoaded Texture
				}
			
				// Load new texture from file
				Texture* makeNewTexture = new Texture(textureFileName, type);
				_mapTexturesWithFileName.insert(std::make_pair(textureFileName, makeNewTexture));

				return makeNewTexture;
			};


		private:
			std::map<std::string, Texture*> _mapTexturesWithFileName;

	};

}

#endif