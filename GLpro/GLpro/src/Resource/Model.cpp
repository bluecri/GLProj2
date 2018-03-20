#include "./Model.h"

namespace RESOURCE 
{
	Model::Model(const std::string &fileName)
	{
		genVao();
		bind();

		loadObjFile(fileName);
		createStructVertexAndIndice();
		createVboEbo();

		unbind();
	}

	Model::~Model()
	{
		glDeleteBuffers(1, &_vbo);
		glDeleteBuffers(1, &_ebo);

		glDeleteVertexArrays(1, &_vao);
	}

	void Model::bind() const
	{
		glBindVertexArray(_vao);
	}

	void Model::unbind() const
	{
		glBindVertexArray(0);
	}

	void Model::genVao()
	{
		glGenVertexArrays(1, &_vao);
	}

	// {vertices, uvs, normals} -> {structVertices, indices}
	void Model::createStructVertexAndIndice()
	{
		createVBOWithVertex(out_vertices, out_uvs, out_normals, out_indices, out_structVertexes);
	}

	// {file} -> {vertices, uvs, normals}
	bool Model::loadObjFile(const std::string &fileName)
	{
		bool res = loadOBJ(fileName.c_str(), out_vertices, out_uvs, out_normals);
		if (!res) {
			printf("%s load obj fail\n", fileName);
			return false;
		}
	}

	//{structVertices, indices} -> vbo, ebo
	void Model::createVboEbo()
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

		// EBO buffer bind
		glGenBuffers(1, &_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, out_indices.size() * sizeof(out_indices[0]), out_indices.data(), GL_STATIC_DRAW);
	}
}
