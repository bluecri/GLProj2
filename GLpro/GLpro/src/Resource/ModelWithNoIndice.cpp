#include "./ModelWithNoIndice.h"

namespace RESOURCE
{
	ModelWithNoIndice::ModelWithNoIndice(const std::string &fileName)
	{
		genVao();
		bind();

		loadObjFile(fileName);
		createStructVertex();
		createVbo();

		unbind();
	}

	ModelWithNoIndice::~ModelWithNoIndice()
	{
		glDeleteBuffers(1, &_vbo);

		glDeleteVertexArrays(1, &_vao);
	}

	void ModelWithNoIndice::bind() const
	{
		glBindVertexArray(_vao);
	}

	void ModelWithNoIndice::unbind() const
	{
		glBindVertexArray(0);
	}

	void ModelWithNoIndice::genVao()
	{
		glGenVertexArrays(1, &_vao);
	}

	// {vertices, uvs, normals} -> {structVertices}
	void ModelWithNoIndice::createStructVertex()
	{
		Struct_Vertex tempVertexWithTexture;

		for (int i = 0; i < out_vertices.size(); i++) {
			tempVertexWithTexture.vertice = out_vertices[i];
			tempVertexWithTexture.uv = out_uvs[i];
			tempVertexWithTexture.normal = out_normals[i];
			out_structVertexes.push_back(tempVertexWithTexture);
		}
	}

	// {file} -> {vertices, uvs, normals}
	bool ModelWithNoIndice::loadObjFile(const std::string &fileName)
	{
		bool res = loadOBJ(fileName.c_str(), out_vertices, out_uvs, out_normals);
		if (!res) {
			printf("%s load obj fail\n", fileName);
			return false;
		}
	}

	//{structVertices, indices} -> vbo
	void ModelWithNoIndice::createVbo()
	{
		// VBO buffer bind
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, out_structVertexes.size() * sizeof(out_structVertexes[0]), out_structVertexes.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Struct_Vertex), (void*)(0));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Struct_Vertex), (void*)(sizeof(float) * 3));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Struct_Vertex), (void*)(sizeof(float) * 5));
	}
}