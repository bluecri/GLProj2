#pragma once

#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "../../3rdParty/TextureLoader/textureLoader.h"

class SkyboxObj {
	public:
		float m_skybox_distance;
		enum ENUM_TEXTURE_IDX {
			UP = 0, DOWN, LEFT, RIGHT, FRONT, BACK
		};
		GLuint textureID[6];

		SkyboxObj(const char* up_path, const char* down_path, const char* left_path, const char* right_path, const char* front_path, const char* back_path, float skybox_distance = 40.0f) {
			textureID[UP] = loadDDS(up_path);
			textureID[DOWN] = loadDDS(down_path);
			textureID[LEFT] = loadDDS(left_path);
			textureID[RIGHT] = loadDDS(right_path);
			textureID[FRONT] = loadDDS(front_path);
			textureID[BACK] = loadDDS(back_path);

			m_skybox_distance = skybox_distance;

			for (int i = 0; i < 6; i++) {
				glBindTexture(GL_TEXTURE_2D, textureID[i]);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			}
			
		}
		~SkyboxObj() {
			for (int i = 0; i < 6; i++) {
				glDeleteTextures(1, &textureID[i]);
			}
	}
};