#include <iostream>


#include "window.h"

int main() {
	GWindow = new WINDOW::Window();
	GWindow->init();
	GWindow->mainLoop();
		
	return 0;
}