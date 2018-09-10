#pragma once

#include "stdafx.h"

namespace RESOURCE
{
	class ModelOnlyVertex
	{
	public:
		ModelOnlyVertex(const std::vector<glm::vec3> &vertices, bool createEbo = false);

		virtual ~ModelOnlyVertex();

		void bind() const;
		void unbind() const;

		void render(GLenum drawMode = GL_TRIANGLES);
		int getGLCount();
		bool isEBO();

	private:
		void genVao();

		std::vector<glm::vec3> _out_vertices;

		virtual void createVboOrEbo(bool createEbo);
		void createBuffer();
		void createStructVertexAndIndice();

	private:
		GLuint	_vao;		//vertex array

		bool	_bEbo;		// whether using ebo
		GLuint	_ebo;		//element buffer
		GLuint	_vbo;		//vertex buffer

		std::vector<Struct_OnlyVertex> _vertices;
		std::vector<unsigned short> _elemVertices;

	};

}
