#pragma once

#include "serializer.hpp"

serializer::draw_func_t fill_polygon(
	const kgeo::polygon_t& polygon,
	const kgeo::draw_range_t& r,
	double line_step = -1
);

inline serializer::draw_func_t fill_triangle(
	const kgeo::point_t& p0, const kgeo::point_t& p1, const kgeo::point_t& p2,
	const kgeo::draw_range_t& r, double step_line = -1
)
{
	return fill_polygon({ p0,p1,p2 }, r, step_line);
}

