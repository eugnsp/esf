// #pragma once
// #include "base.hpp"
// #include "iterator.hpp"
// #include <es/geom/point.hpp>
// #include <es/geom/linear_grid.hpp>
// #include <es_util/iterator.hpp>
// #include <es_util/error.hpp>
// #include <cstddef>
// #include <utility>
// #include <vector>
// #include <string>
// #include <iosfwd>
//
// namespace es_fe
// {
// class Mesh1
// {
// 	friend class View<Vertex_tag, Mesh1>;
// 	friend class View<Edge_tag, Mesh1>;
//
// public:
// 	static constexpr std::size_t dim = 1;
//
// public:
// 	//using Index = mesh::Index;
// 	//using Local_index = mesh::Local_index;
//
// 	using Vertex_view = View<Vertex_tag, Mesh1>;
// 	using Edge_view = View<Edge_tag, Mesh1>;
// 	using Cell_view = Edge_view;
//
// 	using Vertex_iter = Random_access_iterator<Vertex_tag, Mesh1>;
// 	using Edge_iter = Random_access_iterator<Edge_tag, Mesh1>;
// 	using Cell_iter = Edge_iter;
//
// public:
// 	Mesh1() = default;
// 	Mesh1(double x, const geom::Linear_grid& y_grid);
// 	Mesh1(const geom::Linear_grid& x_grid, double y);
//
// 	Index n_vertices() const;
// 	Index n_edges() const;
// 	Index n_cells() const;
// 	Index n_elements(Vertex_tag) const;
// 	Index n_elements(Edge_tag) const;
//
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
// 	//////////////////////////////////////////////////////////////////////////
// 	/** Views */
//
// 	Vertex_view vertex_view(Index) const;
// 	Edge_view edge_view(Index) const;
//
// 	//////////////////////////////////////////////////////////////////////////
// 	/** Iterators */
//
// 	Vertex_iter begin_vertex() const;
// 	Vertex_iter end_vertex() const;
//
// 	es::util::Iterable<Vertex_iter> vertices() const;
//
// 	Edge_iter begin_edge() const;
// 	Edge_iter end_edge() const;
//
// 	es::util::Iterable<Edge_iter> edges() const;
// 	es::util::Iterable<Cell_iter> cells() const;
//
// 	//////////////////////////////////////////////////////////////////////////
//
// 	std::string type_string() const;
//
// private:
// 	void clear();
//
// 	// Adds a new vertex and returns its index
// 	//Index add_vertex(double);
//
// private:
// 	std::vector<geom::Point> vertices_;
// };
//
// std::ostream& operator<<(std::ostream&, const Mesh1&);
// }