#pragma once

#include <string>

#include "./SkyboxObj.h"

#include "../objectAndTexture/Vertex.h"
#include "../shader/ShaderSkybox.h"

class SkyboxObjManager {
	public:
		GLuint m_skyboxVertexID;
		GLuint m_skyboxVertexBufferID;

		glm::mat4 m_modelMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_ProjectionMatrix;

		enum ENUM_SKYBOX_SHADER {
			DEFAULT = 0
		};
		std::vector<ShaderSkybox*> shaderSkyboxVec;	//shader list
		std::vector<std::pair<std::string, std::string>> loadShaderFilePath = { {"shader/SkyboxMapping.vertexshader", "shader/SkyboxMapping.fragmentshader" } };
		int selectedSkyboxShaderIdx = 0;

		enum ENUM_SKYBOX_TEXTURE {
			UNIV = 0
		};

		//std::vector<std::vector<std::string>> loadFilePath = { {"texture/skybox_up.dds", "texture/skybox_down.dds", "texture/skybox_left.dds", "texture/skybox_right.dds", "texture/skybox_front.dds", "texture/skybox_back.dds" } };
		std::vector<std::vector<std::string>> loadFilePath = { { "texture/sky_bot.dds", "texture/sky_top.dds", "texture/sky_back.dds", "texture/sky_front.dds", "texture/sky_right.dds", "texture/sky_left.dds" } };
		std::vector<SkyboxObj*> skyboxObjStorageVec;
		int selectedSkyboxTextureIdx = 0;

		SkyboxObjManager(){
			for (int i = 0; i < loadFilePath.size(); i++) {
				skyboxObjStorageVec.push_back(new SkyboxObj(loadFilePath[i][0].c_str(), loadFilePath[i][1].c_str(), loadFilePath[i][2].c_str(), loadFilePath[i][3].c_str(), loadFilePath[i][4].c_str(), loadFilePath[i][5].c_str(), 50.0f));
			}
		};

		void bufferInit() {
			//load shaders
			for (int i = 0; i < loadShaderFilePath.size(); i++) {
				ShaderSkybox * tempShaderSkybox = new ShaderSkybox(loadShaderFilePath[i].first.c_str(), loadShaderFilePath[i].second.c_str());
				if (tempShaderSkybox->getShaderID() != -1) {
					shaderSkyboxVec.push_back(tempShaderSkybox);
				}
			}
			float d = skyboxObjStorageVec[0]->m_skybox_distance;

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
			std::vector<glm::vec2> skyboxIndices
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

			std::vector<Struct_VertexWithTexture> make_buffer;

			for (int i = 0; i < 24; i++) {
				Struct_VertexWithTexture tempStruct_VertexWithTexture;
				tempStruct_VertexWithTexture.vertice = skyboxVertices[i];
				tempStruct_VertexWithTexture.uv = skyboxIndices[i % 4];
				tempStruct_VertexWithTexture.normal = skyboxNormals[i / 4];
				make_buffer.push_back(tempStruct_VertexWithTexture);
			}

			glGenBuffers(1, &m_skyboxVertexBufferID);

			glGenVertexArrays(1, &m_skyboxVertexID);
			glBindVertexArray(m_skyboxVertexID);

			glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVertexBufferID);
			glBufferData(GL_ARRAY_BUFFER, make_buffer.size() * sizeof(Struct_VertexWithTexture), (void*)&(make_buffer[0]), GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			//glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVertexBufferID);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Struct_VertexWithTexture), (void*)0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Struct_VertexWithTexture), (void*)(sizeof(glm::vec3)));
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Struct_VertexWithTexture), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));


			for (int i = 0; i < 6; i++) {
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, skyboxObjStorageVec[selectedSkyboxTextureIdx]->textureID[i]);
			}
			glBindVertexArray(0);
		}

		void drawSkyBox() {
			glUseProgram(shaderSkyboxVec[selectedSkyboxShaderIdx]->m_shaderID);
			glm::mat4 tempMVP = m_ProjectionMatrix * m_viewMatrix * m_modelMatrix;
			glUniformMatrix4fv(shaderSkyboxVec[selectedSkyboxShaderIdx]->m_modelMatrixID, 1, GL_FALSE, &m_modelMatrix[0][0]);
			glUniformMatrix4fv(shaderSkyboxVec[selectedSkyboxShaderIdx]->m_cameraViewMatrixID, 1, GL_FALSE, &m_viewMatrix[0][0]);
			glUniformMatrix4fv(shaderSkyboxVec[selectedSkyboxShaderIdx]->m_MVPMatrixID, 1, GL_FALSE, &tempMVP[0][0]);

			glDepthMask(GL_FALSE);

			//glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVertexBufferID);
			glBindVertexArray(m_skyboxVertexID);
			glActiveTexture(GL_TEXTURE0);
			for (int i = 0; i < 6; i++) {
				//glBindTexture(GL_TEXTURE_2D, skyboxObjStorageVec[selectedSkyboxTextureIdx]->textureID[i]);
				glUniform1i(shaderSkyboxVec[selectedSkyboxShaderIdx]->m_textureID, 0);
				glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
			}
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
		}

		void setUniformModelMatrix(glm::mat4 &modelMatrix) {
			m_modelMatrix = modelMatrix;
		}
		void setUniformModelMatrixWithDivide(glm::mat4 &modelMatrix, float div) {
			m_modelMatrix = modelMatrix;
			m_modelMatrix[3][0] /= div;
			m_modelMatrix[3][1] /= div;
			m_modelMatrix[3][2] /= div;
		}
		
		void setUniformViewMatrix(glm::mat4 &viewMatrix) {
			m_viewMatrix = viewMatrix;
		}
		void setUniformProjectionMatrix(glm::mat4 &projectionMatrix) {
			m_ProjectionMatrix = projectionMatrix;
		}

		~SkyboxObjManager() {
			for (int i = 0; i < skyboxObjStorageVec.size(); i++) {
				delete skyboxObjStorageVec[i];
			}
			for (int i = 0; i < shaderSkyboxVec.size(); i++) {
				delete shaderSkyboxVec[i];
			}
		}
};