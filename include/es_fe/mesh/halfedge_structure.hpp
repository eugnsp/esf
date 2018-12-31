#pragma once
#include <es_fe/types.hpp>
#include <es_fe/geom/point.hpp>

#include <es_util/error.hpp>

#include <array>
#include <cstddef>
#include <type_traits>
#include <vector>

namespace es_fe::internal
{
// Low-level half-edge datastructure
class Halfedge_structure
{
public:
	using Cell_index = Face_index;

	struct Vertex
	{
		explicit Vertex(const Point& pt) : point(pt)
		{}

		Vertex(const Point& pt, Halfedge_index halfedge) : point(pt), halfedge(halfedge)
		{}

		Point point;

		// An arbitrary halfedge that starts at the vertex (for a boundary
		// vertex this must always be the outer (boundary) half-edge)
		Halfedge_index halfedge = Halfedge_index::invalid;
	};

	struct Halfedge
	{
		explicit Halfedge(Vertex_index vertex) : vertex(vertex)
		{}

		Halfedge(Vertex_index vertex, Halfedge_index next_halfedge, Face_index face) :
			vertex(vertex), next(next_halfedge), face(face)
		{}

		// Index of the vertex the half-edge points to
		Vertex_index vertex;

		// Index of the next half-edge
		Halfedge_index next = Halfedge_index::invalid;

		// Index of the face the half-edge belongs to
		Face_index face = Face_index::invalid;
	};

	struct Face
	{
		explicit Face(Halfedge_index halfedge) : halfedge(halfedge)
		{}

		// One of the half-edges bounding the face
		Halfedge_index halfedge;
	};

public:
	// 	template<class Circ_tag>
	// 	Halfedge_circulator<Circ_tag> halfedge_circ(Halfedge_index) const;

	//////////////////////////////////////////////////////////////////////////
	/** Capacity */

	Vertex_index n_vertices() const;
	Halfedge_index n_halfedges() const;
	Edge_index n_edges() const;
	Face_index n_faces() const;
	Cell_index n_cells() const;

	template<class Tag>
	auto n_elements(Tag) const
	{
		if constexpr (std::is_same_v<Tag, Vertex_tag>)
			return n_vertices();
		else if constexpr (std::is_same_v<Tag, Halfedge_tag>)
			return n_halfedges();
		else if constexpr (std::is_same_v<Tag, Edge_tag>)
			return n_edges();
		else if constexpr (std::is_same_v<Tag, Face_tag>)
			return n_faces();
		else if constexpr (std::is_same_v<Tag, Cell_tag>)
			return n_cells();
	}

	void reserve(Index n_vertices, Index n_edges = 0, Index n_faces = 0);

	void shrink();

	// Returns approximate total size of memory in bytes occupied by the data structure
	std::size_t memory_size() const;

	//////////////////////////////////////////////////////////////////////////
	/** Element access */

	const Point& vertex(Vertex_index vertex) const
	{
		return vertices_[*vertex].point;
	}

	//////////////////////////////////////////////////////////////////////////
	/** Low level iteration */

	Halfedge_index prev(Halfedge_index, Vertex_in_circ_tag) const;
	Halfedge_index prev(Halfedge_index, Vertex_out_circ_tag) const;
	Halfedge_index prev(Halfedge_index, Face_circ_tag) const;

	Halfedge_index next(Halfedge_index, Vertex_in_circ_tag) const;
	Halfedge_index next(Halfedge_index, Vertex_out_circ_tag) const;
	Halfedge_index next(Halfedge_index, Face_circ_tag) const;

	// Traverses a half-edge loop once and calls the given function
	// 		first			the half-edge to start the loop with
	// 		fn				the callback function, should have the signature
	//						equivalent to void(Halfedge_index)
	//		tag				vertex/face-based circulation tag
	template<typename Fn, class Circ_tag>
	void for_each(Halfedge_index first, Fn&& fn, Circ_tag tag) const
	{
		const auto false_pred = [&fn](auto halfedge) {
			fn(halfedge);
			return false;
		};

		find_if(first, false_pred, tag);
	}

	// Traverses a half-edge loop and returns the halfedge index for which the predicate
	// returns true; if the predicate returns true for no halfedge, the invalid index is returned
	// 		first			the half-edge to start the loop with
	// 		pred			the predicate, should have the signature equivalent
	//						to bool(Halfedge_index)
	//		tag				vertex/face-based circulation tag
	template<typename Pred, class Circ_tag>
	Halfedge_index find_if(Halfedge_index first, Pred&& pred, Circ_tag tag) const
	{
		const auto start = first;

		do
		{
			if (pred(first))
				return first;
			first = next(first, tag);
		} while (first != start);

		return Halfedge_index::invalid;
	}

	//////////////////////////////////////////////////////////////////////////

	Vertex_index find_vertex(const Point& point) const;
	Halfedge_index find_halfedge(Vertex_index vertex_from, Vertex_index vertex_to) const;

	Halfedge_index halfedge_index(Vertex_index vertex) const
	{
		return vertices_[*vertex].halfedge;
	}

	Halfedge_index halfedge_index(Face_index face) const
	{
		return faces_[*face].halfedge;
	}

	Vertex_index vertex_index(Halfedge_index halfedge) const
	{
		return halfedges_[*halfedge].vertex;
	}

	Face_index face_index(Halfedge_index halfedge) const
	{
		return halfedges_[*halfedge].face;
	}

	//////////////////////////////////////////////////////////////////////////
	/** Flags */

	// Checks whether the vertex is a boundary one (the function also returns true if
	// the vertex has an invalid edge index; this can happen only during mesh construction
	// when the vertex is added, but the containing element has not yet been created)
	bool is_boundary(Vertex_index) const;
	bool is_boundary(Edge_index) const;
	bool is_boundary(Halfedge_index) const;
	bool is_boundary(Face_index) const;

	// Checks whether the halfedge structure is empty
	// (i.e. if it has either no vertices, or no edges, or no faces)
	bool is_empty() const
	{
		return vertices_.empty() || halfedges_.empty() || faces_.empty();
	}

	//////////////////////////////////////////////////////////////////////////

	void adjust_outgoing_halfedge(Vertex_index vertex)
	{
		find_if(vertices_[*vertex].halfedge,
				[vertex, this](Halfedge_index edge) {
					if (is_boundary(edge))
					{
						vertices_[*vertex].halfedge = edge;
						return true;
					}
					return false;
				},
				Vertex_out_circ_tag{});
	}

	// Adds a vertex and returns the index of the newly added vertex
	Vertex_index add_vertex(const Point& point)
	{
		vertices_.emplace_back(point);
		return n_vertices() - 1;
	}

	// Adds a cell and returns the index of the newly added cell (the function
	// automatically creates the missing edges and adjusts the data structure properly),
	Face_index add_cell(const std::array<Vertex_index, 3>&);

	Halfedge_index first_boundary_halfedge() const
	{
		for (Halfedge_index i{0}; i < n_halfedges(); ++i)
			if (!is_valid(halfedges_[*i].face))
				return i;

		return Halfedge_index::invalid;
	}

	// Clears vertices, edges and cells
	void clear()
	{
		vertices_.clear();
		halfedges_.clear();
		faces_.clear();
	}

	// Performs some basic checks of mesh data structure consistency
	es_util::Error check() const;

	// TODO : generic
	void debug_check_index(Vertex_index index) const
	{
		assert(index < n_vertices() || !is_valid(index));
	}

	void debug_check_index(Edge_index index) const
	{
		assert(index < n_edges() || !is_valid(index));
	}

	void debug_check_index(Halfedge_index index) const
	{
		assert(index < n_halfedges() || !is_valid(index));
	}

	void debug_check_index(Face_index index) const
	{
		assert(index < n_faces() || !is_valid(index));
	}

private:
	// Adds a pair of half-edges without adjusting the data structure, returns
	// the index of the first half-edge in the newly added pair (the first (second)
	// half-edge points to the vertex2 (vertex1)
	Halfedge_index insert_halfedges_raw(Vertex_index vertex1, Vertex_index vertex2)
	{
		halfedges_.emplace_back(vertex1);
		halfedges_.emplace_back(vertex2);
		return n_halfedges() - 2;
	}

	// Adds a cell without adjusting the data structure and
	// returns the index of the newly added cell
	Face_index add_face_raw(Halfedge_index halfedge)
	{
		faces_.emplace_back(halfedge);
		return n_faces() - 1;
	}

protected:
	std::vector<Vertex> vertices_;
	std::vector<Halfedge> halfedges_;
	std::vector<Face> faces_;
};
} // namespace es_fe::internal
