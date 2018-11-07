#pragma once
#include <es/fe/config.hpp>
#include <es/fe/forward.hpp>
#include <es/fe/types.hpp>
#include <es/fe/type_traits.hpp>
#include <es/fe/mesh/view/base.hpp>

//#include <es/geom/point.hpp>
#include <es/geom/tags.hpp>
#include <es/geom/traits.hpp>

#include <array>
#include <utility>

namespace fe
{
template<>
class View<Face_tag, Mesh2> : public internal::View_base<Face_tag, Mesh2>
{
public:
	using Vertex_indices = std::array<Vertex_index, 3>;
	using Halfedge_indices = std::array<Halfedge_index, 3>;
	using Edge_indices = std::array<Edge_index, 3>;
	using Edge_with_dir_indices = std::array<std::pair<Edge_index, bool>, 3>;

public:
	using Vertex_circ = Circulator<Vertex_tag, Face_circ_tag>;
	using Halfedge_circ = Circulator<Halfedge_tag, Face_circ_tag>;
	using Edge_circ = Circulator<Edge_tag, Face_circ_tag>;
	using Face_circ = Circulator<Face_tag, Face_circ_tag>;

public:
	using View_base::View_base;
 
	Halfedge_index halfedge() const;

	Vertex_circ vertex_circ() const;
	Halfedge_circ halfedge_circ() const;
	Edge_circ edge_circ() const; 
 	Face_circ face_circ() const;

	void get_indices(Vertex_indices&) const;
	void get_indices(Vertex_indices&, Edge_indices&) const;
	void get_indices(Vertex_indices&, Halfedge_indices&) const;
	void get_indices(Vertex_indices&, Edge_with_dir_indices&) const;
	
	void get_indices(Halfedge_index, Vertex_indices&, Halfedge_indices&) const;

// 	Vertex_index vertex_index(Local_index) const;
// 	void vertex_indices(std::vector<Vertex_index>&) const;
// 	void vertex_and_edge_indices(std::vector<Vertex_index>& vertices, std::vector<Edge_index>& edges) const;
// 
// 	// edges[i].second is true (false) if the corresponding edge view
// 	// has this face on the left (right)
// 	void vertex_and_edge_indices(std::vector<Vertex_index>& vertices,
// 		std::vector<std::pair<Edge_index, bool>>& edges) const;
// 
// #ifdef TRIANGULAR_CELLS_ONLY
// 	void vertex_indices(Vertex_indices&) const;
// 	void edge_indices(Edge_indices&) const;
// 	void vertex_and_edge_indices(Vertex_indices& vertices, Edge_indices& edges) const;
// 
// 	// edges[i].second is true (false) if the corresponding edge view
// 	// has this face on the left (right)
// 	void vertex_and_edge_indices(Vertex_indices& vertices, Directed_edge_indices& edges) const;
// #endif
// 
// 	/** Returns a vertex by the local index of the vertex in the cell */
// 	const geom::Point& vertex(Local_index index) const;
// 
// #ifdef TRIANGULAR_CELLS_ONLY
// 	template<Local_index index>
// 	const geom::Point& vertex() const
// 	{
// 		static_assert(index < 3, "Index out of bounds");
// 		return vertex(index);
// 	}
// #endif
};
}

namespace geom::internal
{
template<>
struct Traits<fe::View<fe::Face_tag, fe::Mesh2>>
{
	using Index = fe::Local_index;
	using Tag = Triangle_tag;
};
}