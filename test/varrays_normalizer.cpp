
#include "stdafx.h"
#include "varrays_normalizer.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////
// Members Functions
//////////////////////////////////////////////////////////////////////

size_t varrays_normalizer::Normalize()
{
	typedef std::vector<index *> indices_pointers;
	typedef std::map<index, indices_pointers, cmp_lt_vertex> vertex_map;
	typedef vertex_map::const_iterator vertex_map_const_iterator;


	if (m_Mesh.m_VArrays[gl_mesh::vertex].empty() || m_Mesh.m_VArrays[gl_mesh::normal].empty() || m_Mesh.m_VArrays[gl_mesh::texture_coord0].empty())
		throw ("Normalizer: one of the vertex arrays is empty (need vertex/normal/texcoord0)");


	cmp_lt_vertex CmpFct(
		m_Mesh.m_VArrays[gl_mesh::vertex],
		m_Mesh.m_VArrays[gl_mesh::normal],
		m_Mesh.m_VArrays[gl_mesh::texture_coord0]
	);

	// Vertex Map: 
	// - key = position of a vertex in the vertex arrays
	// - contains the list of indices referencing to that vertex
	// - ensure there aren't duplicate vertex entries
	//   (two keys that are different by position will be equal for the map
	//    if they lead to equivalent vertices)
	vertex_map VertexMap(CmpFct);


	// Go tru all submeshes and all indices and put the indices into the map
	// Note: unreference vertices will eliminated that way
	for (size_t i = 0; i < m_Mesh.m_SubMeshes.size(); ++i) {

		const indices & Indices = m_Mesh.m_SubMeshes[i].Indices();

		for (size_t j = 0; j < Indices.size(); ++j)
			VertexMap[Indices[j]].push_back((index *) &(Indices[j]));
	}


	// Create new vertex arrays and remap the indices
	size_t NewVASize = VertexMap.size();

	varray Vertex;		Vertex.reserve(NewVASize);
	varray Normal;		Normal.reserve(NewVASize);
	varray TexCoord;	TexCoord.reserve(NewVASize);

	varray & OldVertex = m_Mesh.m_VArrays[gl_mesh::vertex];
	varray & OldNormal = m_Mesh.m_VArrays[gl_mesh::normal];
	varray & OldTexCoord = m_Mesh.m_VArrays[gl_mesh::texture_coord0];

	for (vertex_map_const_iterator It = VertexMap.begin(); It != VertexMap.end(); ++It) {

		const index VAPos = (* It).first;
		const indices_pointers & IndicesPtr = (* It).second;
		const index NewIndex = index(Vertex.size() / 3);

		Vertex.push_back(OldVertex[VAPos * 3 + 0]);
		Vertex.push_back(OldVertex[VAPos * 3 + 1]);
		Vertex.push_back(OldVertex[VAPos * 3 + 2]);

		Normal.push_back(OldNormal[VAPos * 3 + 0]);
		Normal.push_back(OldNormal[VAPos * 3 + 1]);
		Normal.push_back(OldNormal[VAPos * 3 + 2]);

		TexCoord.push_back(OldTexCoord[VAPos * 2 + 0]);
		TexCoord.push_back(OldTexCoord[VAPos * 2 + 1]);

		for (indices_pointers::const_iterator IndIt = IndicesPtr.begin(); IndIt != IndicesPtr.end(); ++IndIt)
			** IndIt = NewIndex;
	}


	// Replace the old arrays with the new ones
	std::swap(Vertex, OldVertex);
	std::swap(Normal, OldNormal);
	std::swap(TexCoord, OldTexCoord);

	return NewVASize;
}



bool varrays_normalizer::cmp_lt_vertex::operator () (const index & a, const index & b) const {
	
	// To find out whether the vertex referenced by a is less than the vertex referenced by b,
	// we compare each coordinates of the 2 vertices one at the time.
	// This can be seen as a comparison of two strings where the strings hold
	// coordinates instead of characters.

	// Compare the vertex coords
	if (m_Vertex[a * 3 + 0] < m_Vertex[b * 3 + 0])
		return true;
	else if (m_Vertex[a * 3 + 0] > m_Vertex[b * 3 + 0])
		return false;

	else if (m_Vertex[a * 3 + 1] < m_Vertex[b * 3 + 1])
		return true;
	else if (m_Vertex[a * 3 + 1] > m_Vertex[b * 3 + 1])
		return false;

	else if (m_Vertex[a * 3 + 2] < m_Vertex[b * 3 + 2])
		return true;
	else if (m_Vertex[a * 3 + 2] > m_Vertex[b * 3 + 2])
		return false;

	// Compare the normal vector coords
	if (m_Normal[a * 3 + 0] < m_Normal[b * 3 + 0])
		return true;
	else if (m_Normal[a * 3 + 0] > m_Normal[b * 3 + 0])
		return false;

	else if (m_Normal[a * 3 + 1] < m_Normal[b * 3 + 1])
		return true;
	else if (m_Normal[a * 3 + 1] > m_Normal[b * 3 + 1])
		return false;

	else if (m_Normal[a * 3 + 2] < m_Normal[b * 3 + 2])
		return true;
	else if (m_Normal[a * 3 + 2] > m_Normal[b * 3 + 2])
		return false;

	// Compare the texture coords
	if (m_TexCoord[a * 2 + 0] < m_TexCoord[b * 2 + 0])
		return true;
	else if (m_TexCoord[a * 2 + 0] > m_TexCoord[b * 2 + 0])
		return false;

	else if (m_TexCoord[a * 2 + 1] < m_TexCoord[b * 2 + 1])
		return true;
	else if (m_TexCoord[a * 2 + 1] > m_TexCoord[b * 2 + 1])
		return false;

	return false;
}
