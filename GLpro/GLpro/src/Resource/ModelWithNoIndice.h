#ifndef __MODEL_WITH_NO_INDICE_H__
#define __MODEL_WITH_NO_INDICE_H__

#include <glm/glm.hpp>

#include "GL/glew.h"
#include <string>

#include "../../3rdParty/ObjectLoader/objectLoader.h"
#include "../../3rdParty/ObjectLoader/vboindexer.h"

namespace RESOURCE
{
	class ModelWithNoIndice {
	public:
		ModelWithNoIndice(const std::string &fileName);
		virtual ~ModelWithNoIndice();

		void bind() const;
		void unbind() const;
		void genVao();

	protected:
		ModelWithNoIndice() {};			// prevent to call empty constructor. Can called from child.

		std::vector<glm::vec3> out_vertices;
		std::vector<glm::vec2> out_uvs;
		std::vector<glm::vec3> out_normals;

		virtual void createVbo();

	private:

		GLuint _vao;		//vertex array
		GLuint _vbo;		//vertex buffer

		std::vector<Struct_Vertex> out_structVertexes;

		void createStructVertex();
		bool loadObjFile(const std::string &fileName);

	};

}
#endif