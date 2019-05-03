#pragma once
#include <es_fe/geometry/point1.hpp>
#include <es_fe/mesh/iterator/random_access.hpp>
#include <es_fe/mesh/view/edge_mesh1.hpp>
#include <es_fe/mesh/view/vertex_mesh1.hpp>
#include <es_fe/types.hpp>

#include <es_util/iterator.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>
// #include <string>
// #include <iosfwd>

namespace es_fe
{
template<>
class Mesh<1>
{
public:
	static constexpr std::size_t dim = 1;

public:
	using Cell_index = es_fe::Edge_index;
	using Vertex_view = Element_view<Vertex_tag, Mesh>;
	using Edge_view = Element_view<Edge_tag, Mesh>;
	using Cell_view = Edge_view;

	using Vertex_iter = Random_access_iterator<Vertex_tag, Mesh>;
	using Edge_iter = Random_access_iterator<Edge_tag, Mesh>;
	using Cell_iter = Edge_iter;

public:
	Mesh() = default;

	Mesh(std::vector<es_fe::Point1> vertices) : vertices_(std::move(vertices))
	{
		assert(std::is_sorted(vertices_.begin(), vertices_.end()));
	}

	//////////////////////////////////////////////////////////////////////
	//* Capacity */

	es_fe::Vertex_index n_vertices() const;
	es_fe::Edge_index n_edges() const;
	Cell_index n_cells() const;

	es_fe::Vertex_index n_elements(Vertex_tag) const;
	es_fe::Edge_index n_elements(Edge_tag) const;
	Cell_index n_elements(Cell_tag) const;

	bool is_empty() const;

	//////////////////////////////////////////////////////////////////////

	es_fe::Vertex_index find_vertex(es_fe::Point1) const;

	std::pair<es_fe::Vertex_index, es_fe::Vertex_index> vertex_indices(es_fe::Edge_index) const;

	// 	void reserve(Index n_vertices);
	//
	// 	// Returns approximate total size (in bytes) of memory occupied by the mesh
	// 	std::size_t memory_size() const;
	//
	// 	const geom::Point& vertex(Index) const;
	// 	void rescale(double);
	//
	// 	bool is_empty() const;
	// 	bool is_bnd_vertex(Index) const;
	// 	bool is_bnd_cell(Index) const;
	//
	// 	// Performs some basic checks of mesh data structure consistency
	// 	es::util::Error is_ok() const;
	//
	//////////////////////////////////////////////////////////////////////////
	/** Element access */

	const es_fe::Point1& vertex(es_fe::Vertex_index vertex) const
	{
		return vertices_[*vertex];
	}

	//////////////////////////////////////////////////////////////////////////
	/** Flags */

	bool is_boundary(es_fe::Vertex_index) const;
	bool is_boundary(es_fe::Edge_index) const;

	// 	//////////////////////////////////////////////////////////////////////////
	// 	/** Views */
	//
	// 	Vertex_view vertex_view(Index) const;
	// 	Edge_view edge_view(Index) const;
	//
	//////////////////////////////////////////////////////////////////////////
	/** Iterators */

	Vertex_iter begin_vertex() const;
	Vertex_iter end_vertex() const;
	es_util::Iterable<Vertex_iter> vertices() const;

	Edge_iter begin_edge() const;
	Edge_iter end_edge() const;
	es_util::Iterable<Edge_iter> edges() const;

	Cell_iter begin_cell() const;
	Cell_iter end_cell() const;
	es_util::Iterable<Cell_iter> cells() const;

	//////////////////////////////////////////////////////////////////////////
	//
	// 	std::string type_string() const;
	//
	// private:
	// 	void clear();
	//
	// 	// Adds a new vertex and returns its index
	// 	//Index add_vertex(double);
	//
private:
	std::vector<es_fe::Point1> vertices_;
};
//
// std::ostream& operator<<(std::ostream&, const Mesh1&);
} // namespace es_fe
