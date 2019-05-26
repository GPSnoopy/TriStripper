
#ifndef HEADER_GUARD_VARRAYS_NORMALIZER_H
#define HEADER_GUARD_VARRAYS_NORMALIZER_H

#include "gl_mesh.h"



class varrays_normalizer
{
public:
	varrays_normalizer(gl_mesh & Mesh);

	size_t Normalize();

private:
	typedef unsigned int index;
	typedef std::vector<index> indices;
	typedef std::vector<float> varray;

	class cmp_lt_vertex
	{
	public:
		cmp_lt_vertex(const varray & Vertex, const varray & Normal, const varray & TexCoord);
		bool operator () (const index & a, const index & b) const;

	private:
		const varray & m_Vertex;
		const varray & m_Normal;
		const varray & m_TexCoord;
	};

	gl_mesh & m_Mesh;
};




//////////////////////////////////////////////////////////////////////////
// varrays_normalizer Inline functions
//////////////////////////////////////////////////////////////////////////

inline varrays_normalizer::varrays_normalizer(gl_mesh & Mesh) : m_Mesh(Mesh) { }


inline varrays_normalizer::cmp_lt_vertex::cmp_lt_vertex(const varray & Vertex, const varray & Normal, const varray & TexCoord)
	: m_Vertex(Vertex), m_Normal(Normal), m_TexCoord(TexCoord) { }



#endif // HEADER_GUARD_VARRAYS_NORMALIZER_H
