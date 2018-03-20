#ifndef __MODEL_WITH_NO_INDICE_NO_FILE_H__
#define __MODEL_WITH_NO_INDICE_NO_FILE_H__

#include "./ModelWithNoIndice.h"

namespace RESOURCE
{
	class ModelWithNoIndiceNoFile : public ModelWithNoIndice
	{
	public:
		ModelWithNoIndiceNoFile(std::vector<glm::vec3> &vertices, std::vector<glm::vec2> & uvs, std::vector<glm::vec3> &normals);
		virtual ~ModelWithNoIndiceNoFile() {};
	};
}

#endif