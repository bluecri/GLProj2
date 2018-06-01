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
		*	particle의 texture & particle Contatiner & particle sorting & particle generate 담당 클래스
		*	particle의 생성 정보(life, color, size...)는 ParticleCreateInfo에서 담당.
		*	개별 particle(+info)는 ParticleStruct.
		*/
		class ParticleFObj : public FObj
		{
		public:

			ParticleFObj(const char* textureFileName, const char* textureType, int particleContainerSize = PARTICLE_CONTAINER_DEFAULT_SIZE);
			ParticleFObj(const char* textureFileName, const char* textureType, std::vector<glm::vec3> &vertices, int particleContainerSize = PARTICLE_CONTAINER_DEFAULT_SIZE);
			~ParticleFObj();

			void sortContainerByDist();		// camera와 dist로 sorting
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
			int _particleContainerSize;		// 개별 particle container size

			
		private:
			bool _bDeleted;
			float _deleteRemainTime;
		};
		
	}
}
