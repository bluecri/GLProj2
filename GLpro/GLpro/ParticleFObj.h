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
		*	particle�� texture & particle Contatiner & particle sorting & particle generate ��� Ŭ����
		*	particle�� ���� ����(life, color, size...)�� ParticleCreateInfo���� ���.
		*	���� particle(+info)�� ParticleStruct.
		*/
		class ParticleFObj : public FObj
		{
		public:

			ParticleFObj(const char* textureFileName, const char* textureType, int particleContainerSize = DEFAULT_PARTICLE_INBUFFER_DEFAULT_NUM);
			ParticleFObj(const char* textureFileName, const char* textureType, std::vector<glm::vec3> &vertices, int particleContainerSize = DEFAULT_PARTICLE_INBUFFER_DEFAULT_NUM);
			~ParticleFObj();

			void				sortContainerByDist();			// camera�� dist�� sorting
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
			int		_particleContainerSize;		// ���� particle container size

			// int		_curParticleNum;			// ���� ������ �ִ� particleNum(for dynamic buffer size)

		private:
			bool	_bOverUseParticle;		// print particle num > _particleContainerSize

			friend class ParticleFObjManager;
		};
	}
}
