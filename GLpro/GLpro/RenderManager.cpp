#include "RenderManager.h"

#include "OctreeForFrustum.h"
#include "DeferredGFBO.h"

RENDER::RenderManager* GRendermanager = nullptr;;

// todo : acc ó��
void RENDER::RenderManager::renderAll(float deltaTime, float acc)
{
	// ============= draw init (clear buffer) ============= //
	renderBufferInit();


	// ============= normal draw ============= //
	_normalContainer.render(deltaTime);


	// ============= skybox &  particle draw ============= //
	GDeferredGFBO->bindGFBO_RESULT();

	GDeferredGFBO->modeForSkybox();
	if (!GOption->_offSkybox)
		_skyboxContainer.render(deltaTime);

	GDeferredGFBO->modeForParticle();

	if(!GOption->_offParticle)
		_particleContainer.render(deltaTime);
	
	GDeferredGFBO->unbindGFBO_RESULT();


	// ============= post process draw ============= //
	GDeferredGFBO->postProcessDraw(deltaTime);

	// ============= gameui box ============= //
	// gameui text


	// ============= box(ui) draw ============= //
	_boxContainer.render(deltaTime);


	// ============= text draw ============= //
	_textContainer.render(deltaTime);

}

void RENDER::RenderManager::doFrustumTest(float deltaTime, float acc)
{
	// normal object frustum update
	for (auto elem : _normalContainer._rRenderContainer)
		elem.second->updateRRender();

	// normal object frustum test
	/*
	for (auto elem : _normalContainer._rRenderContainer)
	{
		GOctreeForFrustum->newlyInsertComponent(elem.second->getDrawElemList());
	}
	*/

	GOctreeForFrustum->updateOctreeForFrustum();
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
	/*
	if (!GOption->_oldLightUse)
	{
		for (auto elem : _rRenderContainer)
			elem.second->shadowBufferPreDraw(deltaTime);

		for (auto elem : _rRenderContainer)
			elem.second->shadowMappingDraw(deltaTime);
	}
	*/

	// deferred shading
	GDeferredGFBO->modeForShadowDraw();
	for (auto elem : _rRenderContainer)
	{
		GDeferredGFBO->deferredPreDraw(deltaTime, *elem.second);
	}

	GDeferredGFBO->modeForGeoDraw();
	for (auto elem : _rRenderContainer)
	{
		// ============= collision line draw IN ============= //
		GDeferredGFBO->deferredDraw(deltaTime, *elem.second);
	}


	GDeferredGFBO->modeForAfterDraw();
	GDeferredGFBO->deferredAfterDraw(deltaTime);
}