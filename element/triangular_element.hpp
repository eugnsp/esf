#pragma once
#include <es/fe/types.hpp>
#include <es/fe/element/linear_element.hpp>

namespace fe::internal
{
// Base class for a 2D triangular element with (vertex_dofs) internal dofs per vertex,
// (edge_dofs) internal dofs per edge and (face_dofs) internal dofs per face
template<Local_index vertex_dofs, Local_index edge_dofs, Local_index face_dofs>
class Triangular_element : private Linear_element<vertex_dofs, edge_dofs>
{
private:
	using Base = Linear_element<vertex_dofs, edge_dofs>;

public:
	static constexpr Local_index dim = 2;

	using Base::n_dofs;
	using Base::n_total_dofs;

	using Base::n_vertex_dofs;
	using Base::n_edge_dofs;
	using Base::n_total_edge_dofs;
	
	using Base::has_vertex_dofs;
	using Base::has_edge_dofs;

	//////////////////////////////////////////////////////////////////////////
	// Returns the number of internal face dofs
	static constexpr Local_index n_face_dofs = face_dofs;
	static constexpr Local_index n_cell_dofs = n_face_dofs;

	// Returns the number of total edge dofs (vertex + edge + face dofs)
	static constexpr Local_index n_total_face_dofs = 3 * vertex_dofs + 3 * edge_dofs + face_dofs;
	static constexpr Local_index n_total_cell_dofs = n_total_face_dofs;

	// Returns the number of internal face dofs
	static constexpr Local_index n_dofs(Face_tag)
	{
		return n_face_dofs;
	}

	static constexpr Local_index n_dofs(Cell_tag)
	{
		return n_face_dofs;
	}

	// Returns the number of total edge dofs (vertex + edge + face dofs)
	static constexpr Local_index n_total_dofs(Face_tag)
	{
		return n_total_face_dofs;
	}
	
	static constexpr Local_index n_total_dofs(Cell_tag)
	{
		return n_total_face_dofs;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// Checks if the element has internal face dofs
	static constexpr bool has_face_dofs = (n_face_dofs > 0);

	// Checks if the element has dofs of the given type
	template<class Tag>
	static constexpr bool has_dofs(Tag tag)
	{
		return n_dofs(tag) > 0;
	}
};
}