#ifndef __MODEL_FROM_NOT_FILE_H__
#define __MODEL_FROM_NOT_FILE_H__

#include "./Model.h"

namespace RESOURCE
{
	class ModelFromNotFile : public Model
	{
		public:
			ModelFromNotFile(std::vector<glm::vec3> &vertices, std::vector<glm::vec2> & uvs, std::vector<glm::vec3> &normals);
			virtual ~ModelFromNotFile() {};
	};
}

#endif