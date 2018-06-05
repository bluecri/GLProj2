#include "RenderManager.h"

#include "OctreeForFrustum.h"

RENDER::RenderManager* GRendermanager = nullptr;;

// todo : acc Ã³¸®
void RENDER::RenderManager::renderAll(float deltaTime, float acc)
{
	//draw init (clear buffer)
	renderBufferInit();

	// skybox draw
	_skyboxContainer.render(deltaTime);

	// normal draw frustum cull
	for (auto elem : _normalContainer._rRenderContainer)
	{
		GOctreeForFrustum->insertSharedDrawElem(elem.second->getDrawElemList());
	}
	GOctreeForFrustum->setFrustumBitWithMainCamera();
	GOctreeForFrustum->clearPotentialCompPropa();

	// normal draw
	_normalContainer.render(deltaTime);

	// particle draw
	_particleContainer.render(deltaTime);

	//todo gameui box
	// gameui text

	// box(ui) draw
	_boxContainer.render(deltaTime);

	// text draw
	_textContainer.render(deltaTime);


}

void RENDER::RenderManager::swapRenderBuffer()
{
	// Swap buffers
	glfwSwapBuffers(GWindow->_pWindow);
}

template<>
void RENDER::RRenderContainerClass<RENDER::RNormal, SHADER::ShaderMain>::render(float deltaTime)
{
	if (GOption->_oldLightUse)
	{
		for (auto elem : _rRenderContainer)
			elem.second->shadowBufferPreDraw(deltaTime);

		for (auto elem : _rRenderContainer)
			elem.second->shadowBufferDraw(deltaTime);
	}

}