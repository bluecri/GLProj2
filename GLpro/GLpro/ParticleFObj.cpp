#include "stdafx.h"
#include "ParticleFObj.h"
#include "src/Resource/TextureManager.h"
#include "ParticleBuffer.h"

#include "configs.h"


RENDER_TARGET::PARTICLE::ParticleFObj::ParticleFObj(const char * textureFileName, const char * textureType, int particleContainerSize)
{
	_deleteRemainTime = -1.0f;

	std::vector<glm::vec3> default_particle_buffer;
	for (int i = 0; i < 4; i++)
	{
		default_particle_buffer.push_back(glm::vec3(g_vertex_buffer_data[i * 3 + 0], g_vertex_buffer_data[i * 3 + 1], g_vertex_buffer_data[i * 3 + 2]));
	}

	_texture = GTextureManager->getTextureWithFileName(textureFileName, textureType);
	_particleBuffer = new RESOURCE::ParticleBuffer(default_particle_buffer);

	_particleContainerSize = particleContainerSize;
	_particleContainer = std::vector<ParticleStruct*>();
	_particleContainer.reserve(_particleContainerSize);

	for (int i = 0; i < _particleContainerSize; i++)
	{
		_particleContainer.push_back(new ParticleStruct());
	}
}

RENDER_TARGET::PARTICLE::ParticleFObj::ParticleFObj(const char * textureFileName, const char * textureType, std::vector<glm::vec3>& vertices, int particleContainerSize)
	: FObj()
{
	_deleteRemainTime = -1.0f;

	_texture = GTextureManager->getTextureWithFileName(textureFileName, textureType);
	_particleBuffer = new RESOURCE::ParticleBuffer(vertices);

	_particleContainerSize = particleContainerSize;
	_particleContainer = std::vector<ParticleStruct*>();
	_particleContainer.reserve(_particleContainerSize);

	for (int i = 0; i < _particleContainerSize; i++)
	{
		_particleContainer.push_back(new ParticleStruct());
	}
}

RENDER_TARGET::PARTICLE::ParticleFObj::~ParticleFObj()
{
	delete _particleBuffer;

	for (auto elem : _particleContainer)
	{
		delete elem;
	}
}

void RENDER_TARGET::PARTICLE::ParticleFObj::sortContainerByDist()
{
	// needCheck
	std::sort(_particleContainer.begin(), _particleContainer.end(),
	[](const ParticleStruct* lhs, const ParticleStruct* rhs) -> bool
	{
		return lhs->_cameradistance > rhs->_cameradistance;
	});
	
}

void RENDER_TARGET::PARTICLE::ParticleFObj::orderFillParticleBuffer()
{
	int& drawPCnt = _particleBuffer->_particlePrintCnt;
	// todo camera dist < 0.0, no push?
	for (auto elem : _particleContainer)
	{
		if (elem->_life > 0.0f && drawPCnt < MAX_PARTICLE_INBUFFER_DEFAULT_NUM)
		{
			// direct access

			_particleBuffer->_particule_position_data[4 * drawPCnt + 0] = elem->_pos.x;
			_particleBuffer->_particule_position_data[4 * drawPCnt + 1] = elem->_pos.y;
			_particleBuffer->_particule_position_data[4 * drawPCnt + 2] = elem->_pos.z;
			_particleBuffer->_particule_position_data[4 * drawPCnt + 3] = elem->_size;

			_particleBuffer->_particule_color_data[4 * drawPCnt + 0] = elem->_color[0];
			_particleBuffer->_particule_color_data[4 * drawPCnt + 1] = elem->_color[1];
			_particleBuffer->_particule_color_data[4 * drawPCnt + 2] = elem->_color[2];
			_particleBuffer->_particule_color_data[4 * drawPCnt + 3] = elem->_color[3];
			drawPCnt++;
		}
	}
	
	return;
}

ParticleStruct & RENDER_TARGET::PARTICLE::ParticleFObj::GetUnusedParticle()
{
	// find unUsed Particle [lastUsedindex ~ last Index]
	for (int i = _lastUsedParticleIndex; i<_particleContainerSize; i++) {
		if (_particleContainer[i]->_life < 0.0f) {
			_lastUsedParticleIndex = i;
			return *_particleContainer[i];
		}
	}

	// find unUsed Particle [0 ~ lastUsedindex]
	for (int i = 0; i<_lastUsedParticleIndex; i++) {
		if (_particleContainer[i]->_life < 0) {
			_lastUsedParticleIndex = i;
			return *_particleContainer[i];
		}
	}

	return *_particleContainer[0]; // All particles are taken, override the first one
}

void RENDER_TARGET::PARTICLE::ParticleFObj::setDeleteRemainTime(float remainTime)
{
	_deleteRemainTime = remainTime;
}

float & RENDER_TARGET::PARTICLE::ParticleFObj::getDeleteRemainTimeRef() {
	return _deleteRemainTime;
}

