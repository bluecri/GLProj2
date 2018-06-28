#pragma once
#include "stdafx.h"
#include "ParticleStruct.h"

#include "Fobj.h"

#define DEFAULT_PARTICLE_INBUFFER_DEFAULT_NUM 3000

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

			ParticleFObj(const char* textureFileName, const char* textureType, int particleContainerSize = DEFAULT_PARTICLE_INBUFFER_DEFAULT_NUM);
			ParticleFObj(const char* textureFileName, const char* textureType, std::vector<glm::vec3> &vertices, int particleContainerSize = DEFAULT_PARTICLE_INBUFFER_DEFAULT_NUM);
			~ParticleFObj();

			void				sortContainerByDist();			// camera와 dist로 sorting
			void				orderFillParticleBuffer();		// fill live particle pos, color buffer with _particleContainer
			ParticleStruct&		GetUnusedParticle();

			void				accParticleContainderSize(int acc);

			int					getPrevPrintedParticleNum();

			void				bind();
			void				unBind();
			void				renderBuffer();
			void				resetOveruseParticle();

			// update exist particles
			void				updateParticleStructs(float deltaTime, glm::vec3& camPosVec);

		private:
			ParticleFObj() {};

		protected:
			RESOURCE::ParticleBuffer *		_particleBuffer = nullptr;
			RESOURCE::Texture *				_texture = nullptr;

			std::vector<ParticleStruct*>	_particleContainer;

			int		_lastUsedParticleIndex;
			int		_particleContainerSize;		// 개별 particle container size

			// int		_curParticleNum;			// 현재 가지고 있는 particleNum(for dynamic buffer size)

		private:
			bool	_bOverUseParticle;		// print particle num > _particleContainerSize

			friend class ParticleFObjManager;
		};
	}
}
