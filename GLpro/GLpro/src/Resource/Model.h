#ifndef __MODEL_H__
#define __MODEL_H__

#include <glm/glm.hpp>

#include "GL/glew.h"
#include <string>

#include "../../3rdParty/ObjectLoader/objectLoader.h"
#include "../../3rdParty/ObjectLoader/vboindexer.h"

namespace RESOURCE
{
	class Model {
	public:
		Model(const std::string &fileName);
		virtual ~Model();

		void bind() const;
		void unbind() const;
		void genVao();

	protected:
		Model() {};			// prevent to call empty constructor. Can called from child.

		std::vector<glm::vec3> out_vertices;
		std::vector<glm::vec2> out_uvs;
		std::vector<glm::vec3> out_normals;

		virtual void createVboEbo();
		void createStructVertexAndIndice();

	private:

		GLuint _vao;		//vertex array
		GLuint _ebo;		//element buffer
		GLuint _vbo;		//vertex buffer

		std::vector<Struct_Vertex> out_structVertexes;
		std::vector<unsigned short> out_indices;

		bool loadObjFile(const std::string &fileName);

	};

}

#endif