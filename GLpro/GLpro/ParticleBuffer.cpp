#include "stdafx.h"
#include "ParticleBuffer.h"

RESOURCE::ParticleBuffer::ParticleBuffer(std::vector<glm::vec3>& vertices)
{
	_out_vertices = vertices;

	genVao();
	bind();

	createBuffer();

	unbind();
}

RESOURCE::ParticleBuffer::~ParticleBuffer()
{
	glDeleteBuffers(1, &_vbo);
	glDeleteVertexArrays(1, &_vao);
}

void RESOURCE::ParticleBuffer::bind() const
{
	glBindVertexArray(_vao);
}

void RESOURCE::ParticleBuffer::unbind() const
{
	glBindVertexArray(0);
}

void RESOURCE::ParticleBuffer::render()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int printParticleCountOnBuffer = min(MAX_PARTICLE_INBUFFER_DEFAULT_NUM, _particlePrintCnt);

	glBindBuffer(GL_ARRAY_BUFFER, _particles_pos_vbo);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLE_INBUFFER_DEFAULT_NUM * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);  // Buffer orphaning, to improve streaming
	glBufferSubData(GL_ARRAY_BUFFER, 0, printParticleCountOnBuffer * sizeof(GLfloat) * 4, _particule_position_data);

	glBindBuffer(GL_ARRAY_BUFFER, _particles_color_vbo);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLE_INBUFFER_DEFAULT_NUM * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, to improve streaming
	glBufferSubData(GL_ARRAY_BUFFER, 0, printParticleCountOnBuffer * sizeof(GLubyte) * 4, _particule_color_data);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, printParticleCountOnBuffer);

	_particlePrintCnt = 0;	// init count

	glDisable(GL_BLEND);
}

int RESOURCE::ParticleBuffer::getGLCount()
{
	return _vertices.size();
}

void RESOURCE::ParticleBuffer::genVao()
{
	glGenVertexArrays(1, &_vao);
}

void RESOURCE::ParticleBuffer::createBuffer()
{
	// VBO buffer bind
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(_vertices[0]), _vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Struct_Vertex), (void*)(0));

	// vbo with position & color
	glGenBuffers(1, &_particles_pos_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _particles_pos_vbo);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLE_INBUFFER_DEFAULT_NUM * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// vbo with the colors(rgba)
	glGenBuffers(1, &_particles_color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _particles_color_vbo);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLE_INBUFFER_DEFAULT_NUM * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : positions of particles' centers
	glBindBuffer(GL_ARRAY_BUFFER, _particles_pos_vbo);
	glVertexAttribPointer(
		1,                                // attribute
		4,                                // size : x + y + z + size (4 float)
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : particles' colors
	glBindBuffer(GL_ARRAY_BUFFER, _particles_color_vbo);
	glVertexAttribPointer(
		2,                                // attribute
		4,                                // size : r + g + b + a => 4
		GL_UNSIGNED_BYTE,	              // type
		GL_TRUE,                          // normalized : the unsigned char[4] will be accessible with a vec4 (floats) in the shader
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// These functions are specific to glDrawArrays*Instanced*.
	// The first parameter is the attribute buffer we're talking about.
	// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml	// from opengl tutorial
	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
	glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

	// 다음 particle을 출력시 vertice는 buffer 이동하지 않는다.
	glBindVertexArray(0);
}
