#include "ModelManager.h"

#include "./Model.h"
#include "../../ModelOnlyVertex.h"
#include "../../config_default_vertex.h"

namespace RESOURCE
{
	ModelManager::~ModelManager()
	{
		std::for_each(_mapModelWithFileName.begin(), _mapModelWithFileName.end(), [](auto &elem)
		{
			delete elem.second;
		});
	};

	void ModelManager::createDefaultModelOnlyVertex()
	{
		addModelOnlyVertexWIthName(defaultVertex_QuadScreeen, "defaultVertex_QuadScreeen");
	}

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

	ModelOnlyVertex* ModelManager::getModelOnlyVertexWIthName(const std::string & modelName, bool createEbo)
	{
		// Check model already loaded
		auto mapTextureName = _mapModelOnlyVertexWithName.find(modelName);

		if (mapTextureName != _mapModelOnlyVertexWithName.end())
		{
			return mapTextureName->second;	// Return preLoaded Texture
		}

		return nullptr;
	};

	bool ModelManager::addModelOnlyVertexWIthName(const std::vector<glm::vec3>& vertices, const std::string & modelName, bool createEbo)
	{
		// Check model already loaded
		auto mapTextureName = _mapModelOnlyVertexWithName.find(modelName);

		if (mapTextureName != _mapModelOnlyVertexWithName.end())
		{
			return false;
		}

		// Create new model

		ModelOnlyVertex* makeNewModel = new ModelOnlyVertex(vertices, createEbo);
		_mapModelOnlyVertexWithName.insert(std::make_pair(modelName, makeNewModel));

		return true;
	};
	
}

RESOURCE::ModelManager* GModelManager = nullptr;
