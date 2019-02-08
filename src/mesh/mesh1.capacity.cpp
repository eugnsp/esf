#include <es_fe/mesh/mesh1.hpp>
#include <es_fe/types.hpp>

namespace es_fe
{
Vertex_index Mesh1::n_vertices() const
{
	return static_cast<Vertex_index>(vertices_.size());
}

Edge_index Mesh1::n_edges() const
{
	if (vertices_.empty())
		return Edge_index{0};
	else
		return static_cast<Edge_index>(vertices_.size() - 1);
}

auto Mesh1::n_cells() const -> Cell_index
{
	return n_edges();
}

Vertex_index Mesh1::n_elements(Vertex_tag) const
{
	return n_vertices();
}

Edge_index Mesh1::n_elements(Edge_tag) const
{
	return n_edges();
}

auto Mesh1::n_elements(Cell_tag) const -> Cell_index
{
	return n_cells();
}
} // es_fe