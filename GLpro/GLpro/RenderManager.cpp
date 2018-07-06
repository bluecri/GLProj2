#include "RenderManager.h"

#include "OctreeForFrustum.h"
#include "DeferredGFBO.h"

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
	//_particleContainer.render(deltaTime);

	//todo gameui box
	// gameui text

	// box(ui) draw
	_boxContainer.render(deltaTime);

	// text draw
	_textContainer.render(deltaTime);

}

void RENDER::RenderManager::renderBufferInit()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, GWindow->_windowWidth, GWindow->_windowHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles
}

void RENDER::RenderManager::swapRenderBuffer()
{
	// Swap buffers
	glfwSwapBuffers(GWindow->_pWindow);
}

template<>
void RENDER::RRenderContainerClass<RENDER::RNormal, SHADER::ShaderMain>::render(float deltaTime)
{
	if (!GOption->_oldLightUse)
	{
		for (auto elem : _rRenderContainer)
			elem.second->shadowBufferPreDraw(deltaTime);

		for (auto elem : _rRenderContainer)
			elem.second->shadowMappingDraw(deltaTime);
	}
	else
	{
		// deferred shading
		for (auto elem : _rRenderContainer)
			elem.second->deferredDraw(deltaTime);
	}
}