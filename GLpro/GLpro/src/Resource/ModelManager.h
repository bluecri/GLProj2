#ifndef __MODEL_MANAGER_H__
#define __MODEL_MANAGER_H__

#include "GL/glew.h"
#include <glm/gtx/transform.hpp>

#include <vector>
#include <cstring>
#include <iostream>
#include <algorithm>

#include "./Model.h"

#include "../../3rdParty/TextureLoader/textureLoader.h"
#include "../../3rdParty/objectLoader/Vertex.h"
#include "../../3rdParty/objectLoader/objectLoader.h"
#include "../../3rdParty/objectLoader/vboindexer.h"


namespace RESOURCE
{
	class ModelManager {
	public:
		//TODO : try to make unique_ptr
		ModelManager() {};
		virtual ~ModelManager()
		{
			std::for_each(_mapModelWithFileName.begin(), _mapModelWithFileName.end(), [](auto &elem)
			{
				delete elem->second;
			});
		};
		Model* getTextureWithFileName(const std::string &textureFileName)
		{
			// Find model already loaded
			auto mapTextureName = _mapModelWithFileName.find(std::string(textureFileName));

			if (mapTextureName != _mapModelWithFileName.end())
			{
				return mapTextureName->second;	// Return preLoaded Texture
			}

			// Load new texture from file
			Model* makeNewModel = new Model(textureFileName);
			_mapModelWithFileName.insert(std::make_pair(textureFileName, makeNewModel));

			return makeNewModel;
		};


	private:
		std::map<std::string, Model*> _mapModelWithFileName;

	};

}

#endif