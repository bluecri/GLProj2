#include "./window.h"

namespace WINDOW
{
	int init(int windowWidth, int windowHeight) {
		_windowWidth = windowWidth;
		_windowHeight = windowHeight;

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

		_pWindow = (glfwCreateWindow(windowWidth, windowHeight, "OPENGL", NULL, NULL));

		if (_pWindow == NULL) {
			fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
			getchar();
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(_pWindow);

		// Get the actual framebuffer size:
		glfwGetFramebufferSize(_pWindow, &windowWidth, &windowHeight);

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
		glfwSetCursorPos(_pWindow, windowWidth / 2, windowHeight / 2);

		glClearColor(0.0f, 0.0f, 0.7f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);

		return 0;
	}

	void exitWindow() {

	}
}
