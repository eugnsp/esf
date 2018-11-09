#pragma once
#include <es_fe/mesh.hpp>

#include <es/geom/compare.hpp>
#include <es/geom/rect.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <string>
#include <vector>
#include <utility>

namespace es_fe
{
namespace internal
{
class Tensor_mesh : public Mesh2
{
public:
	using Grid = std::vector<double>;

public:
	virtual Vertex_index find_vertex(const geom::Point& point) const override
	{
		const auto ix = binary_find(grid_x_, point.x());
		if (ix == grid_x_.end())
			return Vertex_index::invalid;

		const auto iy = binary_find(grid_y_, point.y());
		if (iy == grid_y_.end())
			return Vertex_index::invalid;

		const auto nx = ix - grid_x_.begin();
		const auto ny = iy - grid_y_.begin();
		return static_cast<Vertex_index>(nx + ny * grid_x_.size());
	}

	const Grid& grid_x() const
	{
		return grid_x_;
	}

	const Grid& grid_y() const
	{
		return grid_y_;
	}

	//! Refines the mesh
	// virtual bool refine(const Refine_strategy&) override;

protected:
	Tensor_mesh(Grid grid_x, Grid grid_y) : grid_x_(std::move(grid_x)), grid_y_(std::move(grid_y))
	{
		assert(std::is_sorted(grid_x_.begin(), grid_x_.end()));
		assert(std::is_sorted(grid_y_.begin(), grid_y_.end()));
	}

private:
	static Grid::const_iterator binary_find(const Grid& grid, double value)
	{
		const auto it = std::lower_bound(grid.begin(), grid.end(), value, geom::Is_less{});
		if (it == grid.end() || !geom::is_equal(*it, value))
			return grid.end();
		else
			return it;
	}

protected:
	Grid grid_x_;
	Grid grid_y_;
};
} // namespace internal

// A triangular tensor mesh class
class Tri_tensor_mesh : public internal::Tensor_mesh
{
public:
	// Constructs a triangular tensor mesh from x- and y-gridlines
	// Each rectangle is bisected using Bisection_strategy predicate.
	template<class Bisection_strategy>
	Tri_tensor_mesh(Grid grid_x, Grid grid_y, Bisection_strategy strategy) :
		Tensor_mesh(std::move(grid_x), std::move(grid_y))
	{
		create_mesh(strategy);
	}

	// Constructs a triangular tensor mesh from x- and y-gridlines
	Tri_tensor_mesh(Grid grid_x, Grid grid_y) :
		Tri_tensor_mesh(std::move(grid_x), std::move(grid_y), default_bisection)
	{}

	virtual std::string type_string() const override
	{
		return "2D triangular tensor mesh";
	}

private:
	template<class Bisection_strategy>
	void create_mesh(Bisection_strategy bisection)
	{
		const auto nx = static_cast<Index>(grid_x_.size());
		const auto ny = static_cast<Index>(grid_y_.size());
		assert(nx > 0 && ny > 0);

		const auto num_vertices = nx * ny;
		const auto num_edges = (nx - 1) * (ny - 1) + nx * (ny - 1) + (nx - 1) * ny;
		const auto num_cells = 2 * (nx - 1) * (ny - 1);

		reserve(num_vertices, num_edges, num_cells);

		std::vector<Vertex_index> prev(nx), next(nx);

		for (Index i = 0; i < nx; ++i)
			prev[i] = add_vertex({grid_x_[i], grid_y_[0]});

		for (Index j = 1; j < ny; ++j)
		{
			for (Index i = 0; i < nx; ++i)
				next[i] = add_vertex({grid_x_[i], grid_y_[j]});

			for (Index i = 0; i < nx - 1; ++i)
			{
				const geom::Rect rect({grid_x_[i], grid_y_[j - 1]}, {grid_x_[i + 1], grid_y_[j]});
				if (bisection(rect))
				{                                                //     *---*  next
					add_cell(prev[i], prev[i + 1], next[i + 1]); //     | / |
					add_cell(prev[i], next[i + 1], next[i]);     //     *---*  prev
				}                                                //	   i  i+1
				else
				{                                                //     *---*  next
					add_cell(next[i], prev[i], prev[i + 1]);     //     | \ |
					add_cell(next[i], prev[i + 1], next[i + 1]); //     *---*  prev
				}                                                //	   i  i+1
			}

			swap(prev, next);
		}

		assert(*n_vertices() == num_vertices);
		assert(*n_edges() == num_edges);
		assert(*n_cells() == num_cells);

		// HACK
		// debug_check();
	}

	static bool default_bisection(const geom::Rect&)
	{
		return true;
	}
};
} // namespace es_fe