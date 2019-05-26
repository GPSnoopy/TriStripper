
#ifndef HEADER_GUARD_GL_MODEL_H
#define HEADER_GUARD_GL_MODEL_H

#include "gl_mesh.h"



// 3D Model class
class gl_model
{
public:

	bool LoadFromVXO(std::istream & File);

	size_t NbMeshes() const;

	gl_mesh & Mesh(size_t i);
	const gl_mesh & Mesh(size_t i) const;

	gl_mesh & operator[] (size_t i);
	const gl_mesh & operator[] (size_t i) const;

	void CompileAllMeshes();
	void ExecuteAllMeshes() const;

protected:
	typedef std::vector<gl_mesh> meshes;

	meshes	m_Meshes;
};




//////////////////////////////////////////////////////////////////////////
// gl_mesh Inline functions
//////////////////////////////////////////////////////////////////////////

inline size_t gl_model::NbMeshes() const {
	return m_Meshes.size();
}


inline gl_mesh & gl_model::Mesh(size_t i) {
	return m_Meshes[i];
}


inline const gl_mesh & gl_model::Mesh(size_t i) const {
	return m_Meshes[i];
}


inline gl_mesh & gl_model::operator [] (size_t i) {
	return m_Meshes[i];
}


inline const gl_mesh & gl_model::operator [] (size_t i) const {
	return m_Meshes[i];
}


inline void gl_model::CompileAllMeshes()
{
	for (size_t i = 0; i < NbMeshes(); ++i)
		Mesh(i).Compile();
}


inline void gl_model::ExecuteAllMeshes() const
{
	for (size_t i = 0; i < NbMeshes(); ++i)
		Mesh(i).Execute();
}



#endif // HEADER_GUARD_GL_MODEL_H
