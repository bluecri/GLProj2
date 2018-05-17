#include "./window.h"

#include "Resource\ModelManager.h"
#include "Resource\TextureManager.h"

#include "Transform.h"
#include "../LightManager.h"
#include "Camera\CameraManager.h"

#include "Shader\ShaderManager.h"

#include "../Option.h"
#include "../ShadowBufferTexture.h"

WINDOW::Window::Window(int windowWidth, int windowHeight)
{
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
}

int WINDOW::Window::init()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_pWindow = (glfwCreateWindow(_windowWidth, _windowHeight, "OPENGL_G", NULL, NULL));

	if (_pWindow == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(_pWindow);

	// Get the actual framebuffer size:
	glfwGetFramebufferSize(_pWindow, &_windowWidth, &_windowHeight);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(_pWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwPollEvents();
	glfwSetCursorPos(_pWindow, _windowWidth / 2, _windowHeight / 2);

	glClearColor(0.0f, 0.0f, 0.7f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	GModelManager = new RESOURCE::ModelManager();
	GTextureManager = new RESOURCE::TextureManager();
	GLightManager = new LightManager();
	GCameraManager = new CAMERA::CameraManager();
	GShaderManager = new SHADER::ShaderManager();
	GOption = new Option();

	//
	GShadowBufferTexture = new RESOURCE::ShadowBufferTexture();

	return 0;
}

void WINDOW::Window::draws()
{
	// Draw loop (ESC key or window was closed)
	do {
		//managerOfManager->draws();
	} while (glfwGetKey(_pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(_pWindow) == 0);
}

void WINDOW::Window::exitWindow()
{
}

WINDOW::Window* GWindow = nullptr;

