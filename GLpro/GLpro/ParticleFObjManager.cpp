#include "ParticleFObjManager.h"
#include "ParticleFObj.h"
#include "./src/Resource/Texture.h"

std::shared_ptr<ParticleFObjManager::PFobjElem> ParticleFObjManager::getSharedParticleFObj(const char * textureFileName, const char * textureType)
{
	auto it = _fObjContainer.find(textureFileName);
	if (it != _fObjContainer.end())
	{
		return (*it).second;
	}
	auto newSharedFObj = std::shared_ptr<PFobjElem>(new PFobjElem(textureFileName, textureType));
	_fObjContainer.insert(make_pair(textureFileName, newSharedFObj));

	return newSharedFObj;
}

std::shared_ptr<ParticleFObjManager::PFobjElem> ParticleFObjManager::getSharedParticleFObj(const char * textureFileName, const char * textureType, std::vector<glm::vec3>& vertices)
{
	auto it = _fObjContainer.find(textureFileName);
	if (it != _fObjContainer.end())
	{
		return (*it).second;
	}
	auto newSharedFObj = std::shared_ptr<PFobjElem>(new PFobjElem(textureFileName, textureType, vertices));
	_fObjContainer.insert(make_pair(textureFileName, newSharedFObj));

	return newSharedFObj;
}

void ParticleFObjManager::removeSharedParticleFObj(std::shared_ptr<PFobjElem> delElem)
{
	auto it = _fObjContainer.find(delElem->_texture->getFileName());
	if (it != _fObjContainer.end())
	{
		_fObjContainer.erase(it);
	}
}

// sort with distance & fill ParticleBuffer & particle over use flag init (pos buffer & color buffer)
void ParticleFObjManager::doAllFobjBeforeDrawBehavior()
{
	for (auto elem : _fObjContainer)
	{
		if (elem.second->isBDeleted())
		{
			continue;
		}

		if (!elem.second->isRender())
		{
			continue;
		}
		// sort with distance
		elem.second->sortContainerByDist();

		// fill ParticleBuffer (pos buffer & color buffer)
		elem.second->orderFillParticleBuffer();		// cannot parallel

		elem.second->resetOveruseParticle();
	}
}

void ParticleFObjManager::updateParticleStructsAll(float deltaTime, glm::vec3& camPosVec)
{
	for (auto elem : _fObjContainer)
	{
		elem.second->updateParticleStructs(deltaTime, camPosVec);
	}
}

std::map<std::string, std::shared_ptr<RENDER_TARGET::PARTICLE::ParticleFObj>>& ParticleFObjManager::getFobjContainerForDraw()
{
	return _fObjContainer;
}

ParticleFObjManager* GParticleFObjManager = nullptr;