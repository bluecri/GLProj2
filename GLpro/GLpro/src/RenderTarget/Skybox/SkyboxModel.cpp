#include "SkyboxModel.h"
#include "../../Resource/Model.h"

RENDER_TARGET::SKYBOX::SkyboxModel::SkyboxModel(float skybox_distance)
{
	_skybox_distance = skybox_distance;
	makeModelWithDistance();
}

RENDER_TARGET::SKYBOX::SkyboxModel::~SkyboxModel()
{

}

void RENDER_TARGET::SKYBOX::SkyboxModel::bind()
{
	_skbModel->bind();
}

void RENDER_TARGET::SKYBOX::SkyboxModel::unbind()
{
	_skbModel->unbind();
}

void RENDER_TARGET::SKYBOX::SkyboxModel::makeModelWithDistance()
{
	std::vector<glm::vec3> retSkyboxVertices;
	std::vector<glm::vec2> retSkyboxUVs;
	std::vector<glm::vec3> retSkyboxNormals;
	float d = _skybox_distance;

	std::vector<glm::vec3> skyboxVertices{
		// up down
		glm::vec3(-d, d, -d), glm::vec3(d, d, -d), glm::vec3(-d, d, d), glm::vec3(d, d, d),
		glm::vec3(d, -d, -d), glm::vec3(-d, -d, -d), glm::vec3(d, -d, d), glm::vec3(-d, -d, d),

		// left right
		glm::vec3(-d, d, d), glm::vec3(-d, -d, d), glm::vec3(-d, d, -d), glm::vec3(-d, -d, -d),
		glm::vec3(d, d, -d), glm::vec3(d, -d, -d), glm::vec3(d, d, d), glm::vec3(d, -d, d),

		// front back
		glm::vec3(d, d, d), glm::vec3(d, -d, d), glm::vec3(-d, d, d), glm::vec3(-d, -d, d),
		glm::vec3(-d, d, -d), glm::vec3(-d, -d, -d), glm::vec3(d, d, -d), glm::vec3(d, -d, -d)
	};
	std::vector<glm::vec2> skyboxUVs
	{
		glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f)
	};

	std::vector<glm::vec3> skyboxNormals
	{
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	};

	for (int i = 0; i < 24; i++) {
		retSkyboxVertices.push_back(skyboxVertices[i]);
		retSkyboxUVs.push_back(skyboxUVs[i % 4]);
		retSkyboxNormals.push_back(skyboxNormals[i / 4]);
	}

	_skbModel = new RESOURCE::Model(retSkyboxVertices, retSkyboxUVs, retSkyboxNormals, false);
}
