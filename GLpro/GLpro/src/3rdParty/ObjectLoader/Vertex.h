#pragma once
#include <glm/glm.hpp>

struct Struct_Vertex {
	glm::vec3 vertice;
	glm::vec2 uv;
	glm::vec3 normal;

	bool operator<(const Struct_Vertex other) const
	{
		// memory address compare
		return memcmp((void*)this, (void*)&other, sizeof(Struct_Vertex)) > 0;
	}
};

struct Struct_OnlyVertex {
	glm::vec3 vertice;

	bool operator<(const Struct_OnlyVertex other) const
	{
		// memory address compare
		return memcmp((void*)this, (void*)&other, sizeof(Struct_OnlyVertex)) > 0;
	}
};