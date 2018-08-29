#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>


namespace WINDOW
{
	class Window
	{
	public:
		Window(int windowWidth = 800, int windowHeight = 600);
		int init();
		void mainLoop();
		void renderAll(float usedDeltaTime, float acc);
		void exitWindow();

		void mouseToCenter();
		float getCollisionDt();
	public:
		GLFWwindow* _pWindow;
		int _windowWidth;		// window size
		int _windowHeight;
		const float _collisionDt = 0.02f;
	};
}

extern WINDOW::Window* GWindow;
#endif