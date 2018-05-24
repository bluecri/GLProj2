#include "RenderManager.h"

RENDER::RenderManager* GRendermanager = nullptr;;

// todo : acc Ã³¸®
inline void RENDER::RenderManager::renderAll(float deltaTime, float acc)
{
	// skybox draw
	_skyboxContainer.render(deltaTime);

	// normal draw
	_normalContainer.render(deltaTime);

	// particle draw
	_particleContainer.render(deltaTime);

	// box(ui) draw
	_boxContainer.render(deltaTime);

	// text draw
	_textContainer.render(deltaTime);
}

inline void RENDER::RenderManager::swapRenderBuffer()
{
	// Swap buffers
	glfwSwapBuffers(GWindow->_pWindow);
	glfwPollEvents();
}
