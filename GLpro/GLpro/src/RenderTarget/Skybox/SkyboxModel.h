#ifndef __SKYBOX_VERTEX_GROUP__H__
#define __SKYBOX_VERTEX_GROUP__H__

#include <glm/glm.hpp>

#include <vector>

#include "../../Resource/ModelWithNoIndiceNoFile.h"
/*
skybox vertex group : has distance and ModelWithNoIndiceNoFile.
*/
namespace RENDER_TARGET
{
	namespace SKYBOX
	{
		class SkyboxModel
		{
		public:
			float _skybox_distance;
			RESOURCE::ModelWithNoIndiceNoFile *_skbModel;

			std::vector<glm::vec3> _skyboxVertices;
			std::vector<glm::vec2> _skyboxUVs;
			std::vector<glm::vec3> _skyboxNormals;

			SkyboxModel(float skybox_distance = 40.0f)
			{
				_skybox_distance = skybox_distance;
				makeModelWithDistance();
			}
			virtual ~SkyboxModel()
			{

			}

			void bind() {
				_skbModel->bind();
			}
			void unbind() {
				_skbModel->unbind();
			}

		private:
			void makeModelWithDistance()
			{
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
					_skyboxVertices.push_back(skyboxVertices[i]);
					_skyboxUVs.push_back = skyboxUVs[i % 4];
					_skyboxNormals.push_back = skyboxNormals[i / 4];
				}

				_skbModel = new RESOURCE::ModelWithNoIndiceNoFile(_skyboxVertices, _skyboxUVs, _skyboxNormals);
			}
		}
	}
};
#endif