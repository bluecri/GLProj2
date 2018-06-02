#pragma once

#include "../../stdafx.h"

#define MAX_PARTICLE_INBUFFER_DEFAULT_NUM 3000

namespace RESOURCE
{
	class ParticleBuffer
	{
	public:
		ParticleBuffer(std::vector<glm::vec3> &vertices);

		virtual ~ParticleBuffer();

		void bind() const;
		void unbind() const;
		void render();
		int getGLCount();

		// CAREFUL!
		GLfloat _particule_position_data[MAX_PARTICLE_INBUFFER_DEFAULT_NUM * 4];
		GLubyte _particule_color_data[MAX_PARTICLE_INBUFFER_DEFAULT_NUM * 4];
		int _particlePrintCnt;	//should be updated !
	private:
		void genVao();
		void createBuffer();

	private:
		GLuint	_vao;		//vertex array
		GLuint	_vbo;		//vertex buffer
							
		GLuint	_particles_pos_vbo;		// vbo with position & size
		GLuint	_particles_color_vbo;	// vbo with color(rgba)

		std::vector<glm::vec3> _vertices;
	};

}