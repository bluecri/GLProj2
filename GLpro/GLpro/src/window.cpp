#include "./window.h"

#include "Resource\ModelManager.h"
#include "Resource\TextureManager.h"

#include "Transform.h"
#include "../LightManager.h"
#include "Camera\CameraManager.h"

#include "Shader\ShaderManager.h"

#include "../Option.h"
#include "../ShadowBufferTextureShader.h"
#include "../RenderManager.h"
#include "../Scene.h"

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
	GRendermanager = new RENDER::RenderManager();
	GOption = new Option();
	GScene = new Scene();
	//
	GShadowBufferTexture = new RESOURCE::ShadowBufferTextureShader();

	return 0;
}

void WINDOW::Window::mainLoop()
{
	float t = 0.0;
	const float dt = 0.02;

	float currentTime = glfwGetTime();
	float acc = 0.0;

	// Draw loop (ESC key or window was closed)
	do {
		// time update
		float newTime = glfwGetTime();
		float intervalTime = newTime - currentTime;
		currentTime = newTime;

		acc += intervalTime;

		/*physics loop
			*{
			*rigidbody Comp에서 최상위 transform부터 dirty bit를 이용한 world matrix update 시작하며
			*		delta 존재시 적용후 dirty on(O(n))	// transform 자식으로 collision을 넣어 collision update도 하면서 순회도중에 dirty init 하는 방법 존재.
			*		collision box update에 dirty & world matrix 사용
			*		collision event push & collision message 남김
			*		dirty bit init(O(n))
			* }
			*transform 개별 조작시(logic update) dirty,
			*collision message로 logic update
			*/
		while (acc >= dt)
		{
			// todo : update
			acc -= dt;
			t += dt;
		}

		// dt 보정 2가지 방법
		// 1. 1frame 늦게 출력(past - current사이 정확한 interpolation)
		// 2. acc만큼의 예상 이동 경로 그냥 그리기(acc가 dt에 가까울 수록 interpolation error 증가)
		// 2번으로 시도.
		renderAll(acc);

	} while (glfwGetKey(_pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(_pWindow) == 0);
}

void WINDOW::Window::renderAll(float acc)
{
	// RRenrder all with acc
}

void WINDOW::Window::exitWindow()
{
}

WINDOW::Window* GWindow = nullptr;

