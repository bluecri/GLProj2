#include <iostream>


#include "window.h"

int main() {
	GWindow = new WINDOW::Window();
	GWindow->init();
	GWindow->draws();
		
	return 0;
}