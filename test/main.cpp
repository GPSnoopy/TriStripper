
#include "stdafx.h"

#include "gl_renderer.h"
#include "static_assert.h"



int main(int argc, char * argv[])
{
	// Launch OpenGL Renderer and run the predefined scene
	try {
		gl_renderer();
	}

	// Global error handling
	catch (std::bad_alloc) {
		std::cerr << "!!! Fatal Error !!! Not enough memory!\n" << std::endl;
	}

	catch (const char Error[]) {
		std::cerr << "!!! Fatal Error !!! " << Error << "!\n" << std::endl;
	}
}

