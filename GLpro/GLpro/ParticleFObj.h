#pragma once
#include "stdafx.h"
#include "ParticleStruct.h"

#include "Fobj.h"

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
		/*
		*	particle�� texture & particle Contatiner & particle sorting & particle generate ��� Ŭ����
		*	particle�� ���� ����(life, color, size...)�� ParticleCreateInfo���� ���.
		*	���� particle(+info)�� ParticleStruct.
		*/
		class ParticleFObj : public FObj
		{
		public:

			ParticleFObj(const char* textureFileName, const char* textureType, int particleContainerSize = PARTICLE_CONTAINER_DEFAULT_SIZE);
			ParticleFObj(const char* textureFileName, const char* textureType, std::vector<glm::vec3> &vertices, int particleContainerSize = PARTICLE_CONTAINER_DEFAULT_SIZE);
			~ParticleFObj();

			void sortContainerByDist();		// camera�� dist�� sorting
			void orderFillParticleBuffer();	// fill live particle pos, color buffer with _particleContainer
			ParticleStruct& GetUnusedParticle();

			void setBDeleted();
			void setDeleteRemainTime(float remainTime);
			bool bDeleted();
			float& getDeleteRemainTimeRef();
			
		private:
			ParticleFObj() {};

		public:
			RESOURCE::ParticleBuffer * _particleBuffer = nullptr;
			RESOURCE::Texture * _texture = nullptr;

			std::vector<ParticleStruct*> _particleContainer;
			int _lastUsedParticleIndex;
			int _particleContainerSize;		// ���� particle container size

			
		private:
			bool _bDeleted;
			float _deleteRemainTime;
		};
		
	}
}
