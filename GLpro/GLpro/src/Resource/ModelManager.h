#ifndef __MODEL_MANAGER_H__
#define __MODEL_MANAGER_H__

#include "../../stdafx.h"

namespace RESOURCE { 
	class Model;
	class ModelOnlyVertex;
}
using ModelElemContainer = std::map<std::string, RESOURCE::Model*>;
using ModelOnlyVertexElemContainer = std::map<std::string, RESOURCE::ModelOnlyVertex*>;


namespace RESOURCE
{
	class ModelManager {
	public:
		ModelManager() {};
		virtual ~ModelManager();

		void				createDefaultModelOnlyVertex();
		Model*				getModelWithFileName(const std::string &modelFileName, bool createEbo = true);
		ModelOnlyVertex*	getModelOnlyVertexWIthName(const std::string &modelName, bool createEbo = true);
		bool				addModelOnlyVertexWIthName(const std::vector<glm::vec3> &vertices, const std::string &modelName, bool createEbo = true);

	private:
		ModelElemContainer				_mapModelWithFileName;
		ModelOnlyVertexElemContainer	_mapModelOnlyVertexWithName;
	};
}

extern RESOURCE::ModelManager* GModelManager;

#endif