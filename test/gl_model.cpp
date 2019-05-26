
#include "gl_model.h"

#include "fileio.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
// Members Functions
//////////////////////////////////////////////////////////////////////

bool gl_model::LoadFromVXO(std::istream & File)
{
	using io_facilities::readvar;

	const std::ios::iostate States = File.exceptions();
	File.exceptions(std::ios::badbit | std::ios::eofbit | std::ios::failbit);

	bool Result = true;

	try {
		// Is this really a VXO file?
		char String[18];
		File.read(String, 18);
		if (strcmp(String, "OPENGL ENGINE VXO") != 0)
			File.clear(std::ios::failbit);

		// Number of Meshes
		size_t NbMeshes;
		readvar(File, NbMeshes, 4);
		m_Meshes.resize(NbMeshes);

		// Load Each Mesh
		for (size_t i = 0; i < NbMeshes; ++i)
			Mesh(i).LoadFromVxo(File);
	}

	catch (const std::ios::failure &)
	{
		Result = false;
	}

	File.exceptions(States);

	return Result;
}



