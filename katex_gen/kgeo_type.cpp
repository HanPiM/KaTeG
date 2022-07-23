#include "kdraw.hpp"

kgeo::polygon_t::polygon_t(const unary_func_t& x_oft, const unary_func_t& y_oft, const range_t& t_range, double mindx, double step)
{
	if (mindx > 0)
	{
		adaptive_make_points_from_func_oft(
			x_oft, y_oft, t_range, mindx, *this
		);
		return;
	}
	if (step <= 0)step = t_range.width() / 25.0;
	for (double t = t_range.left(); _kfunc::dleq(t, t_range.right()); t += step)
	{
		emplace_back(x_oft(t), y_oft(t));
	}
}

kgeo::polygon_t::polygon_t(const unary_func_t& x_oft, const unary_func_t& y_oft, const range_t& t_range, const serializer& s, const draw_range_t& r)
{
	adaptive_make_points_from_func_oft(
		x_oft, y_oft, t_range,
		s, r,
		*this
	);
}
