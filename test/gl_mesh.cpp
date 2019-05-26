
#include "gl_mesh.h"
#include "fileio.h"
#include "varrays_normalizer.h"
#include "tri_stripper.h"
#include "high_res_timer.h"



//#define USE_NVIDIA_STRIPPER
#ifdef USE_NVIDIA_STRIPPER
#include "NvTriStrip.h"
#endif



size_t gl_mesh::s_CacheSize = 10;
bool gl_mesh::s_ViewStrips = true;



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

gl_mesh::~gl_mesh()
{
	if (m_DisplayListID != 0)
		glDeleteLists(m_DisplayListID, 1);
}



//////////////////////////////////////////////////////////////////////
// Members Functions
//////////////////////////////////////////////////////////////////////

void gl_mesh::Compile()
{
	std::cout << "Compiling Mesh " << Name() << ": " << std::endl;

	NormalizeVArrays();		// Remove duplicate or useless elements
	TriangleStrip();		// Convert to triangle strips
	InitDisplayList();		// Create a display list for future executions
}



void gl_mesh::Execute() const
{
	// If there is no display list, simply draw the scene
	if (m_DisplayListID == 0)
		RawDraw();

	// else call the display list
	else
		glCallList(m_DisplayListID);
}



void gl_mesh::LoadFromVxo(std::istream & File)
{
	// float must be 32 bits
	static_assert(sizeof(char) == sizeof(uint8_t));
	static_assert(sizeof(float) == sizeof(uint32_t));

	using io_facilities::readvar;
	
	// Mesh Name
	char String[256];
	File.getline(String, 256, '\0');
	m_Name = String;

	// Mesh Info
	size_t NbSubMeshes;
	readvar(File, NbSubMeshes, 4);
	readvar(File, m_NbVertices, 4);
	readvar(File, m_NbTextures, 4);

	m_SubMeshes.resize(NbSubMeshes);
	m_VArrays[vertex].resize(3 * m_NbVertices);
	m_VArrays[normal].resize(3 * m_NbVertices);

	for (size_t i = 0; i < m_NbTextures; ++i)
		m_VArrays[array_type(texture_coord0 + i)].resize(2 * m_NbVertices);

	// Load Mesh Data
	for (size_t i = 0; i < (m_NbVertices * 3); ++i)
		readvar(File, reinterpret_cast<uint32_t &>(m_VArrays[vertex][i]), 4);

	for (size_t i = 0; i < (m_NbVertices * 3); ++i)
		readvar(File, reinterpret_cast<uint32_t &>(m_VArrays[normal][i]), 4);

	for (size_t i = 0; i < m_NbTextures; ++i)
		for (size_t j = 0; j < (m_NbVertices * 2); ++j)
			readvar(File, reinterpret_cast<uint32_t &>(m_VArrays[array_type(texture_coord0 + i)][j]), 4);

	// Load Each SubMesh
	for (size_t i = 0; i < m_SubMeshes.size(); ++i)
		m_SubMeshes[i].LoadFromVXO(File);
}



void gl_mesh::submesh::LoadFromVXO(std::istream & File)
{
	using io_facilities::readvar;

	// SubMesh Render Mode
	readvar(File, m_glMode, 4);

	// SubMesh Size
	size_t NbIndices;
	readvar(File, NbIndices, 4);
	m_Indices.resize(NbIndices);

	// Load Data Types And Acts As Needed
	for (size_t i = 0; i < NbIndices; ++i) readvar(File, m_Indices[i], 4);
}



void gl_mesh::InitDisplayList()
{
	if (m_DisplayListID != 0) {
		glDeleteLists(m_DisplayListID, 1);
		m_DisplayListID = 0;
	}

	m_DisplayListID = glGenLists(1);

	if (m_DisplayListID == 0)
		throw("Couldn't generate a display list");

	glNewList(m_DisplayListID, GL_COMPILE);
		RawDraw();
	glEndList();
}



void gl_mesh::NormalizeVArrays()
{
	using namespace std;

	cout << "- Normalizing vertex arrays: " << endl;

	high_res_timer Timer;

	varrays_normalizer Normalizer(* this);

	size_t NewSize = Normalizer.Normalize();

	cout << "  - went from " << static_cast<unsigned int>(m_NbVertices) << " to " << static_cast<unsigned int>(NewSize) << " vertices." << endl;
	cout << "  - took " << Timer.ElapsedTime<float>() * 1000.0 << " msecs." << endl;
	cout << endl;
}



void gl_mesh::TriangleStrip()
{
	using namespace std;

	// verify there is only one submesh containing only triangles
	if ((m_SubMeshes.size() != 1) || (m_SubMeshes[0].GLMode() != GL_TRIANGLES))
		throw ("trying to triangle strip a mesh that has more than one submesh or doesn't contain triangles");

	cout << "- Stripping triangles: " << endl;
	cout << "  - number of triangles: " << static_cast<unsigned int>(m_SubMeshes[0].Indices().size() / 3) << "." << endl;


#ifndef USE_NVIDIA_STRIPPER

	// USING TRI STRIPPER

	using namespace triangle_stripper;

	high_res_timer Timer;

	// convert 32 bits indices from the submesh into the type used by Tri Stripper
	indices Indices(m_SubMeshes[0].Indices().begin(), m_SubMeshes[0].Indices().end());

	primitive_vector PrimitivesVector;

	{ // we want to time the tri_stripper object destruction as well
		tri_stripper TriStripper(Indices);

		TriStripper.SetMinStripSize(2);
		TriStripper.SetCacheSize(s_CacheSize);
		TriStripper.SetBackwardSearch(false);

		TriStripper.Strip(&PrimitivesVector);
	}

	const float Time = Timer.ElapsedTime<float>() * 1000.0;

	// Replace the submeshes with the new one
	m_SubMeshes.clear();
	m_SubMeshes.reserve(PrimitivesVector.size());

	for (size_t i = 0; i < PrimitivesVector.size(); ++i) {
		
		// convert back from Tri Stripper indices type to 32 bits indices
		const submesh::indices NewIndices(PrimitivesVector[i].Indices.begin(), PrimitivesVector[i].Indices.end());
		
		m_SubMeshes.push_back(submesh(NewIndices, PrimitivesVector[i].Type));
	}

	// Display statistics
	size_t NbStrips = 0;
	size_t NbTriangles = 0;
	size_t NbStrippedTri = 0;

	for (size_t i = 0; i < PrimitivesVector.size(); ++i) {
		if (PrimitivesVector[i].Type == TRIANGLE_STRIP) {
			++NbStrips;
			NbStrippedTri += (PrimitivesVector[i].Indices.size()) - 2;
		} else
			NbTriangles += (PrimitivesVector[i].Indices.size()) / 3;
	}

	cout << "  - generated " << static_cast<unsigned int>(NbStrips) << " triangle strips." << endl;
	cout << "  - stripped " << static_cast<unsigned int>(NbStrippedTri) << " triangles." << endl;
	cout << "  - left " << static_cast<unsigned int>(NbTriangles) << " triangles alone." << endl;
	cout << "  - created " << static_cast<unsigned int>(PrimitivesVector.size()) << " primitive groups." << endl;
	cout << "  - took " << Time << " msecs." << endl;
	cout << endl;


#else

	// USING NVIDIA STRIPPER

	unsigned short NbGroups;
	PrimitiveGroup * pPrimitiveGroup = NULL;
	std::vector<unsigned short> Indices(m_SubMeshes[0].Indices().begin(), m_SubMeshes[0].Indices().end());

	high_res_timer Timer;

	SetCacheSize(s_CacheSize - 6);
	SetStitchStrips(false);
	SetMinStripSize(2);

	GenerateStrips(&(Indices[0]), static_cast<unsigned int>(Indices.size()), &pPrimitiveGroup, &NbGroups);

	const float Time = Timer.ElapsedTime<float>() * 1000.0;

	// Replace the submeshes with the new one
	m_SubMeshes.clear();
	m_SubMeshes.reserve(NbGroups);

	for (size_t i = 0; i < NbGroups; ++i) {

		GLenum Type;

		switch (pPrimitiveGroup[i].type) {
		case PT_LIST:
			Type = GL_TRIANGLES;
			break;
		case PT_STRIP:
			Type = GL_TRIANGLE_STRIP;
			break;
		case PT_FAN:
			Type = GL_TRIANGLE_FAN;
			break;
		}

		m_SubMeshes.push_back(
			submesh(
				submesh::indices(
					pPrimitiveGroup[i].indices + 0,
					pPrimitiveGroup[i].indices + pPrimitiveGroup[i].numIndices
				),
				Type
			)
		);
	}

	// Display statistics
	size_t NbStrips = 0;
	size_t NbFans = 0;
	size_t NbTriangles = 0;
	size_t NbStrippedTri = 0;

	for (size_t i = 0; i < NbGroups; ++i) {

		GLenum Type;

		switch (pPrimitiveGroup[i].type) {
		case PT_LIST:
			Type = GL_TRIANGLES;
			break;
		case PT_STRIP:
			Type = GL_TRIANGLE_STRIP;
			break;
		case PT_FAN:
			Type = GL_TRIANGLE_FAN;
			break;
		}

		if (Type == GL_TRIANGLE_STRIP) {
			++NbStrips;
			NbStrippedTri += (pPrimitiveGroup[i].numIndices) - 2;
		} else if (Type == GL_TRIANGLES)
			NbTriangles += (pPrimitiveGroup[i].numIndices) / 3;
		else if (Type == GL_TRIANGLE_FAN)
			++NbFans;
	}

	cout << "  - generated " << static_cast<unsigned int>(NbFans) << " triangle fans." << endl;
	cout << "  - generated " << static_cast<unsigned int>(NbStrips) << " triangle strips." << endl;
	cout << "  - stripped " << static_cast<unsigned int>(NbStrippedTri) << " triangles." << endl;
	cout << "  - left " << static_cast<unsigned int>(NbTriangles) << " triangles alone." << endl;
	cout << "  - created " << static_cast<unsigned int>(NbGroups) << " primitive groups." << endl;
	cout << "  - took " << Time << " msecs." << endl;
	cout << endl;

#endif
}



void gl_mesh::RawDraw() const
{
	glVertexPointer(3, GL_FLOAT, 0, &(m_VArrays[vertex][0]));
	glNormalPointer(GL_FLOAT, 0, &(m_VArrays[normal][0]));

	glColor3f(0.4, 0.4, 0.4);

	for (size_t i = 0; i < m_SubMeshes.size(); ++i) {

		if (s_ViewStrips) {

			// Switch colors to draw triangle strips
			if (m_SubMeshes[i].GLMode() == GL_TRIANGLE_STRIP) {			
				switch(i % 19) {
				case 0:
					glColor3f(1.00, 0.00, 0.00);
					break;
				case 1:
					glColor3f(0.75, 0.25, 0.00);
					break;
				case 2:
					glColor3f(0.50, 0.50, 0.00);
					break;
				case 3:
					glColor3f(0.25, 0.75, 0.00);
					break;
				case 4:
					glColor3f(0.00, 1.00, 0.00);
					break;
				case 5:
					glColor3f(0.00, 0.75, 0.25);
					break;
				case 6:
					glColor3f(0.00, 0.50, 0.50);
					break;
				case 7:
					glColor3f(0.00, 0.25, 0.75);
					break;
				case 8:
					glColor3f(0.00, 0.00, 1.00);
					break;
				case 9:
					glColor3f(0.25, 0.00, 0.75);
					break;
				case 10:
					glColor3f(0.50, 0.00, 0.50);
					break;
				case 11:
					glColor3f(0.75, 0.00, 0.25);
					break;
				case 12:
					glColor3f(1.00, 0.00, 0.00);
					break;
				case 13:
					glColor3f(1.00, 1.00, 0.00);
					break;
				case 14:
					glColor3f(0.00, 1.00, 0.00);
					break;
				case 15:
					glColor3f(0.00, 1.00, 1.00);
					break;
				case 16:
					glColor3f(0.00, 0.00, 1.00);
					break;
				case 17:
					glColor3f(1.00, 0.00, 1.00);
					break;
				case 18:
					glColor3f(1.00, 1.00, 1.00);
					break;
				}

			} else {
				glColor3f(0.10, 0.10, 0.10);
			}

		}

		glDrawElements(
			m_SubMeshes[i].GLMode(),
			(GLsizei) m_SubMeshes[i].Indices().size(),
			GL_UNSIGNED_INT,
			&((m_SubMeshes[i].Indices())[0]));
	}
		
}

