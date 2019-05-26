
#include "gl_renderer.h"
#include <iostream>
#include <stdexcept>



int main(int argc, char* argv[])
{
	// Launch OpenGL Renderer and run the predefined scene
	try {
		gl_renderer(argc, argv);
	}

	// Global error handling
	catch (const std::bad_alloc&) {
		std::cerr << "!!! Fatal Error !!! Not enough memory!\n" << std::endl;
	}

	catch (const char Error[]) {
		std::cerr << "!!! Fatal Error !!! " << Error << "!\n" << std::endl;
	}
}

