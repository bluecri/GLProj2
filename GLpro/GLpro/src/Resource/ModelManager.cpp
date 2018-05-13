#include "ModelManager.h"

#include "./Model.h"

namespace RESOURCE
{
	ModelManager::~ModelManager()
	{
		std::for_each(_mapModelWithFileName.begin(), _mapModelWithFileName.end(), [](auto &elem)
		{
			delete elem->second;
		});
	};

	Model * ModelManager::getModelWithFileName(const std::string & modelFileName, bool createEbo)
	{
		// Check model already loaded
		auto mapTextureName = _mapModelWithFileName.find(modelFileName);

		if (mapTextureName != _mapModelWithFileName.end())
		{
			return mapTextureName->second;	// Return preLoaded Texture
		}

		// Load new texture from file
		Model* makeNewModel = new Model(modelFileName, createEbo);
		_mapModelWithFileName.insert(std::make_pair(modelFileName, makeNewModel));

		return makeNewModel;
	};

	ModelManager* GModelManager = nullptr;

}