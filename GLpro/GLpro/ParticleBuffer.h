#pragma once

#include "../../stdafx.h"


namespace RESOURCE
{
	class ParticleBuffer
	{
	public:
		ParticleBuffer(std::vector<glm::vec3> &vertices, int particleBufferCapacity);

		virtual ~ParticleBuffer();

		void	bind() const;
		void	unbind() const;
		void	render();
		int		getGLCount();

		int		getBufferParticleCapacity();
		void	accParticleCapacity(int acc);

	private:
		void	genVao();
		void	createBuffer();

	public:
		// CAREFUL!
		std::vector<GLfloat> _particule_position_data;
		std::vector<GLubyte> _particule_color_data;

		int _particlePrintCnt;	//should be updated !

	private:
		GLuint	_vao;		//vertex array
		GLuint	_vbo;		//vertex buffer
		int		_bufferParticleCapacity;
		int		_prevBufferParticleCapacity;
		bool	_bBufferParticleCapacityUpdated;
							
		GLuint	_particles_pos_vbo;		// vbo with position & size
		GLuint	_particles_color_vbo;	// vbo with color(rgba)

		std::vector<glm::vec3> _vertices;
	};

}