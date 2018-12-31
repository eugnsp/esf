#pragma once
#include <es_fe/types.hpp>
#include <es_fe/geom/point.hpp>

#include <es_util/numeric.hpp>

namespace es_fe
{
namespace internal
{
template<class Triangle>
Point circumcentre(const Triangle& triangle, Triangle_tag)
{
	auto&& a = triangle.vertex(0);
	auto&& b = triangle.vertex(1);
	auto&& c = triangle.vertex(2);

	// TODO : make generic, no (-), revice dot and cross
	const auto r2 = b - a;
	const auto r3 = c - a;

	const double r22 = dot(r2, r2);
	const double r32 = dot(r3, r3);
	const double d = 2 * cross2(r2, r3);

	const double x = a.x() + es_util::det(r22, r32, r2[1], r3[1]) / d;
	const double y = a.y() - es_util::det(r22, r32, r2[0], r3[0]) / d;

	return {x, y};
}
} // namespace internal

template<class Geometry>
Point circumcentre(const Geometry& geometry)
{
	return internal::circumcentre(geometry, typename Geometry::Geometry_tag{});
}
} // namespace es_fe
