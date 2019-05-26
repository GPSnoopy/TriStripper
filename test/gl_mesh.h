
#ifndef HEADER_GUARD_GL_MESH_H
#define HEADER_GUARD_GL_MESH_H

#include <GL/glut.h>
#include <string>
#include <vector>


// 3D Mesh class
class gl_mesh
{
public:

	enum array_type {
		vertex				= 0,
		normal				= 1,
		texture_coord0		= 2
	};

	static const size_t MaxVertexArrays = 3;
	static const size_t MaxTextures = 1;

	gl_mesh();
	~gl_mesh();

	void LoadFromVxo(std::istream & File);

	void Compile();
	void Execute() const;
	const std::string & Name() const;

	static size_t s_CacheSize;
	static bool s_ViewStrips;

private:

	friend class varrays_normalizer;

	
	// SubMesh class
	class submesh
	{
	public:
		typedef uint32_t index;
		typedef std::vector<index> indices;

		submesh();
		submesh(const indices & Indices, const GLenum glMode);
		
		void LoadFromVXO(std::istream & File);

		indices & Indices();
		const indices & Indices() const;
		GLenum GLMode() const;

	protected:
		indices		m_Indices;
		GLenum		m_glMode;
	};


	// Private member functions
	void InitDisplayList();
	void NormalizeVArrays();
	void TriangleStrip();

	void RawDraw() const;


	// New private types
	typedef std::vector<submesh>			submeshes;
	typedef std::vector<float>				varray;			// vertex_array
	typedef std::vector<varray>				varrays;		// vertex_arrays


	// Private member variables
	std::string		m_Name;

	varrays			m_VArrays;
	submeshes		m_SubMeshes;

	size_t			m_NbVertices;
	size_t			m_NbTextures;
	GLuint			m_DisplayListID;
};




//////////////////////////////////////////////////////////////////////////
// gl_mesh Inline functions
//////////////////////////////////////////////////////////////////////////

inline gl_mesh::gl_mesh() : m_VArrays(MaxVertexArrays), m_DisplayListID(0) { }


inline const std::string & gl_mesh::Name() const {
	return m_Name;
}



//////////////////////////////////////////////////////////////////////////
// gl_mesh::submesh Inline functions
//////////////////////////////////////////////////////////////////////////

inline gl_mesh::submesh::submesh() { }


inline gl_mesh::submesh::submesh(const indices & Indices, const GLenum glMode) : m_Indices(Indices), m_glMode(glMode) { }


inline GLenum gl_mesh::submesh::GLMode() const {
	return m_glMode;
}


inline gl_mesh::submesh::indices & gl_mesh::submesh::Indices() {
	return m_Indices;
}


inline const gl_mesh::submesh::indices & gl_mesh::submesh::Indices() const {
	return m_Indices;
}



#endif // HEADER_GUARD_GL_MESH_H
