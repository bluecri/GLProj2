#ifndef __MODEL_MANAGER_H__
#define __MODEL_MANAGER_H__

#include <map>
#include <string>

namespace RESOURCE { class Model; }
using ModelElemContainer = std::map<std::string, RESOURCE::Model*>;


namespace RESOURCE
{
	class ModelManager {
	public:
		ModelManager() {};
		virtual ~ModelManager();
		Model* getModelWithFileName(const std::string &modelFileName, bool createEbo = true);
		
	private:
		ModelElemContainer _mapModelWithFileName;
	};
}

extern RESOURCE::ModelManager* GModelManager;

#endif