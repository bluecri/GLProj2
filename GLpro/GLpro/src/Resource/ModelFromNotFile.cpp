#include "./ModelFromNotFile.h"

namespace RESOURCE
{
	ModelFromNotFile::ModelFromNotFile(std::vector<glm::vec3> &vertices, std::vector<glm::vec2> & uvs, std::vector<glm::vec3> &normals)
	{
		out_vertices = vertices;
		out_uvs = uvs;
		out_normals = normals;

		genVao();
		bind();

		createStructVertexAndIndice();
		createVboEbo();

		unbind();
	}
}