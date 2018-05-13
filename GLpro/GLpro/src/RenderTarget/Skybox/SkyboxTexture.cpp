#include "SkyboxTexture.h"
#include "../../Resource/Texture.h"

RENDER_TARGET::SKYBOX::SkyboxTexture::SkyboxTexture(const char * up_path, const char * down_path, const char * left_path, const char * right_path, const char * front_path, const char * back_path)
{
	textureID[UP] = new RESOURCE::Texture(up_path, "DDS");
	textureID[DOWN] = new RESOURCE::Texture(down_path, "DDS");
	textureID[LEFT] = new RESOURCE::Texture(left_path, "DDS");
	textureID[RIGHT] = new RESOURCE::Texture(right_path, "DDS");
	textureID[FRONT] = new RESOURCE::Texture(front_path, "DDS");
	textureID[BACK] = new RESOURCE::Texture(back_path, "DDS");

	for (int i = 0; i < 6; i++) {
		textureID[i]->bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		textureID[i]->unbind();
	}
}

RENDER_TARGET::SKYBOX::SkyboxTexture::~SkyboxTexture()
{
	for (int i = 0; i < 6; i++) {
		delete textureID[i];
	}
}

void RENDER_TARGET::SKYBOX::SkyboxTexture::bind(int idx)
{
	textureID[idx]->bind();
}

void RENDER_TARGET::SKYBOX::SkyboxTexture::unbind(int idx)
{
	textureID[idx]->unbind();
}
