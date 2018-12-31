#pragma once
#include <es_fe/types.hpp>
#include <es_fe/geom/point.hpp>
#include <cassert>

namespace fe
{
class Segment
{
public:
	using Geometry_tag = Segment_tag;

public:
	Segment() = default;

	Segment(const Point& p1, const Point& p2) : points_{p1, p2}
	{
		// TODO
		assert(p1 != p2);
	}

	template<Index index>
	const Point& vertex() const
	{
		static_assert(index < 2, "Index out of bounds");
		return points_[index];
	}

	const Point& vertex(Index index) const
	{
		assert(index < 2);
		return points_[index];
	}

private:
	Point points_[2];
};
} // namespace fe