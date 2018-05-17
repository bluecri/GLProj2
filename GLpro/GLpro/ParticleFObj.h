#pragma once
#include "stdafx.h"
#include "ParticleStruct.h"

#define PARTICLE_CONTAINER_DEFAULT_SIZE		360		//3sec * 2 particle per 1 frame

namespace RESOURCE
{
	class ParticleBuffer;
	class Texture;
}

namespace RENDER_TARGET
{
	namespace PARTICLE
	{
		class ParticleFObj
		{
		public:

			ParticleFObj(const char* textureFileName, const char* textureType, std::vector<glm::vec3> &vertices, int particleContainerSize = PARTICLE_CONTAINER_DEFAULT_SIZE);
			~ParticleFObj();

			void sortContainerByDist();		// camera¿Í dist·Î sorting
			void orderFillParticleBuffer();	// fill live particle pos, color buffer with _particleContainer
			ParticleStruct& GetUnusedParticle();

		public:
			RESOURCE::ParticleBuffer * _particleBuffer = nullptr;
			RESOURCE::Texture * _texture = nullptr;

			std::vector<ParticleStruct*> _particleContainer;
			int _lastUsedParticleIndex;
			int _particleContainerSize;
		private:
			ParticleFObj() {};
		};
		
	}
}
