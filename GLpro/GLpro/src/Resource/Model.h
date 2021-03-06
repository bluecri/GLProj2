#ifndef __MODEL_H__
#define __MODEL_H__

#include "../../stdafx.h"

namespace RESOURCE
{
	class Model 
	{
	public:
		Model(const std::string &fileName, bool createEbo = true);
		Model(std::vector<glm::vec3> &vertices, std::vector<glm::vec2> & uvs, std::vector<glm::vec3> &normals, bool createEbo = false);

		virtual ~Model();

		void bind() const;
		void unbind() const;

		void render();
		int getGLCount();
		bool isEBO();

	private:
		void genVao();

		std::vector<glm::vec3> _out_vertices;
		std::vector<glm::vec2> _out_uvs;
		std::vector<glm::vec3> _out_normals;

		virtual void createVboOrEbo(bool createEbo);
		void createBuffer();
		void createStructVertexAndIndice();

	private:
		GLuint	_vao;		//vertex array

		bool	_bEbo;		// whether using ebo
		GLuint	_ebo;		//element buffer
		GLuint	_vbo;		//vertex buffer

		std::vector<Struct_Vertex> _vertices;
		std::vector<unsigned short> _elemVertices;

		bool loadObjFile(const std::string &fileName);

	};

}

#endif