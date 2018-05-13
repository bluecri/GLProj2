#ifndef __MODEL_MANAGER_H__
#define __MODEL_MANAGER_H__

#include "../../stdafx.h"

class Model;

namespace RESOURCE
{
	class ModelManager {
	public:
		//TODO : try to make shared_ptr is better ?
		ModelManager() {};
		virtual ~ModelManager();
		Model* getModelWithFileName(const std::string &modelFileName, bool createEbo = true);
		
	private:
		std::map<std::string, Model*> _mapModelWithFileName;
	};
}

extern RESOURCE::ModelManager* GModelManager;

#endif