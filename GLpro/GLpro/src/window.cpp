#include "./window.h"

#include "Resource\ModelManager.h"
#include "Resource\TextureManager.h"

#include "../LightManager.h"
#include "Camera\CameraManager.h"

#include "Shader\ShaderManager.h"

#include "../Option.h"
#include "../ShadowBufferTextureShader.h"
#include "../DeferredGFBO.h"

#include "../RenderManager.h"
#include "../CollisionComponentManager.h"
#include "../RigidbodyComponentManager.h"
#include "../src/Control/InputManager.h"
#include "../src/Sound/ALManager.h"
#include "../Scene.h"

#include "../SkyboxGObject.h"
#include "../Box.h"
#include "../ImageBox.h"
#include "../TextBox.h"
#include "../Canvas.h"
#include "../SkyboxGObject.h"
#include "../GameSession.h"

#include "../OctreeForFrustum.h"
#include "../ParticleFObjManager.h"


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
	
	// glClearColor(0.0f, 0.0f, 0.7f, 0.0f);	set clear color
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	GInputManager = new CONTROL::InputManager();
	GRigidbodyComponentManager = new RigidbodyComponentManager();
	GModelManager = new RESOURCE::ModelManager();
	GModelManager->createDefaultModelOnlyVertex();
	GTextureManager = new RESOURCE::TextureManager();
	GCameraManager = new CAMERA::CameraManager();
	GLightManager = new LightManager();
	GParticleFObjManager = new ParticleFObjManager();

	GALManager = new ALManager();
	GALManager->init();
	GOctreeForFrustum = new OctreeForFrustum(4, 128, glm::vec3());
	// ttest
	//GLightManager->AddDirectinalLight(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//GLightManager->AddDirectinalLight(glm::vec3(-10.0f, -10.0f, 10.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));


	GShaderManager = new SHADER::ShaderManager();
	GRendermanager = new RENDER::RenderManager();
	GCollisionComponentManager = new CollisionComponentManager(4, 128);
	GOption = new Option();
	GScene = new Scene();
	
	GShadowBufferTexture = new RESOURCE::ShadowBufferTextureShader();
	GShadowBufferTexture->init();
	GDeferredGFBO = new RESOURCE::DeferredGFBO(_windowWidth, _windowHeight);
	GDeferredGFBO->initDeferredGFBO();

	//load ttest
	Box::initPreMade();
	ImageBox::initPreMade();
	TextBox::initPreMade();
	
	Canvas::initPreMade();
	SkyboxGObject::preMade();

	GameSession::preMade();

	GScene->_targetGameSession = GameSession::preMadeGameSession[0];
	GScene->_targetGameSession->setAllEntityRRender(true);
	//GScene->changeCanvas(
	//SkyboxGObject::preMadeSpaceSkybox[0]->skyboxFObj->setBRender(true);

	
	return 0;
}

void WINDOW::Window::mainLoop()
{
	float t = 0.0f;
	float usedT = 0.0f;
	const float dt = 0.02f;

	float currentTime = static_cast<float>(glfwGetTime());
	float acc = 0.0;

	static double time_lastframe = static_cast<float>(glfwGetTime());
	double time_now;
	int cnt = 0;
	

	// Draw loop (ESC key or window was closed)
	do {
		// time update
		float newTime = static_cast<float>(glfwGetTime());
		float intervalTime = newTime - currentTime;
		currentTime = newTime;

		acc += intervalTime;
		glfwPollEvents();
		GInputManager->keyUpdate();		// transfer keyinput to GScene
		while (acc >= dt)
		{
			/*physics loop
			*{
			* rigidbody Comp에서 최상위 RigidbodyComponent부터 dirty bit를 이용한 world matrix update 시작하며
			*		delta 존재시 적용후 dirty on(O(n))
			*		collision box update에 dirty & world matrix 사용
			*		collision event push & collision message 남김
			*		dirty bit init(O(n))
			* }
			* RigidbodyComponent 개별 조작시(logic update) dirty,
			* collision message로 logic update
			*/
			GRigidbodyComponentManager->updateRigidbodyComps(dt);
			GCollisionComponentManager->doCollisionTest();
			GRigidbodyComponentManager->resetRigidbodyCompsDirty();
			
			// logic loop
			GScene->update(dt, acc);

			// todo : update
			acc -= dt;
			t += dt;
			usedT += dt;

			//GCollisionComponentManager->clearOctree();
		}

		// dt 보정 2가지 방법
		// 1. 1frame 늦게 출력(past - current사이 정확한 interpolation)
		// 2. acc만큼의 예상 이동 경로 그냥 그리기(acc가 dt에 가까울 수록 interpolation error 증가)
		// 2번으로 시도.
		// render loop

		GCameraManager->updateAllRecentMatrix();
		GRendermanager->frustumObjectUpdate(usedT, acc);		// draw object frustum update

		GLightManager->updateAllLIghts();						// light pos update + light frustum object
		GALManager->updateALSource();

		//Sleep(1);
		renderAll(usedT, acc);

		GLightManager->deUpdateAllLIghts();		// light frustum object container clear
		GOctreeForFrustum->clearPotentialCompPropa();	// clear frustum

		usedT = 0.0;


		cnt++;
		time_now = newTime;
		double time_frame = time_now - time_lastframe;
		if (time_frame >= 1.0)
		{
			printf_s("%d\n", cnt);
			cnt = 0;
			time_lastframe = time_lastframe + 1.0;
		}

	} while (glfwGetKey(_pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(_pWindow) == 0);
}

void WINDOW::Window::renderAll(float usedDeltaTime, float acc)
{
	// RRenrder all with acc
	GRendermanager->renderAll(usedDeltaTime, acc);		//render
	GRendermanager->swapRenderBuffer();					// swap render buffer
}

void WINDOW::Window::exitWindow()
{
}

void WINDOW::Window::mouseToCenter()
{
	glfwSetCursorPos(_pWindow, _windowWidth / 2, _windowHeight / 2);
}

WINDOW::Window* GWindow = nullptr;

