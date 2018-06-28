#pragma once

#include "stdafx.h"

namespace RESOURCE
{
	class ParticleBuffer;
	class Texture;
}

namespace RENDER_TARGET
{
	namespace PARTICLE	
	{
		class ParticleFObj;
	}
}

/*
*	ParticleFObjManager
*	If use same texture, then use smae ParticleFObj
*/

class ParticleFObjManager
{
	using PFobjElem = RENDER_TARGET::PARTICLE::ParticleFObj;
public:
	std::shared_ptr<PFobjElem>	getSharedParticleFObj(const char* textureFileName, const char* textureType);
	std::shared_ptr<PFobjElem>	getSharedParticleFObj(const char* textureFileName, const char* textureType, std::vector<glm::vec3> &vertices);
	void						removeSharedParticleFObj(std::shared_ptr<PFobjElem> delElem);
	void						doAllFobjBeforeDrawBehavior();
	void						updateParticleStructsAll(float deltaTime, glm::vec3& camPosVec);

	std::map<std::string, std::shared_ptr<PFobjElem>>&	getFobjContainerForDraw();

private:
	std::map<std::string, std::shared_ptr<PFobjElem>>	_fObjContainer;
};

extern ParticleFObjManager* GParticleFObjManager;