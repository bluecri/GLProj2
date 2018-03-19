#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

namespace WINDOW
{
	GLFWwindow* _pWindow;
	
	int _windowWidth;		// window size
	int _windowHeight;

	int init(int windowWidth, int windowHeight);

	void exitWindow();
}

#endif