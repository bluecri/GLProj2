#pragma once

#include "../../stdafx.h"

namespace RESOURCE
{
	class ShadowBuffer
	{
	public:
		ShadowBuffer(const std::string &fileName, bool createEbo = true);
		ShadowBuffer(std::vector<glm::vec3> &vertices, std::vector<glm::vec2> & uvs, std::vector<glm::vec3> &normals, bool createEbo = false);

		virtual ~Model();

		void bind() const;
		void unbind() const;
		void genVao();

		int getGLCount();
		bool isEBO();

	protected:

		std::vector<glm::vec3> _out_vertices;
		std::vector<glm::vec2> _out_uvs;
		std::vector<glm::vec3> _out_normals;

		virtual void createVboOrEbo(bool createEbo);
		void createBuffer();
		void createStructVertexAndIndice();

	private:
		GLuint	_vao;		//vertex array

		GLuint	_textVBO;		//element buffer
		GLuint	_uvVBO;		//vertex buffer

		std::vector<Struct_Vertex> _vertices;
		std::vector<unsigned short> out_indices;

		bool loadObjFile(const std::string &fileName);

	};

}