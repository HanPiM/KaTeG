#pragma once

#include "serializer.hpp"

inline double defaut_next_step_func(double x) { return x + 1; }

using dvec_t = std::vector<double>;

#include "kdraw_fill.hpp"

enum class border_style
{
	expend_inward,
	expend_middle,
	expend_outward
};

void default_draw_point_func(serializer& s, const kgeo::point_t& p);
inline kunit empty_draw_func(serializer&) { return 0; }

serializer::draw_func_t draw_border(
	kunit w, kunit h, kunit offset = 0_pt,
	border_style style = border_style::expend_inward,
	kunit topw = 0.1_pt, kunit bottomw = 0.1_pt,
	kunit leftw = 0.1_pt, kunit rightw = 0.1_pt
);

// ���� rule ������� ���������� 100 ʱ����ֿɼ����ܿ�����
serializer::draw_func_t draw_grid(
	kgeo::rect_t grid_range,
	const kgeo::point_t& offset_pos, double dx, double dy,
	const kgeo::draw_range_t& r
);

serializer::draw_func_t draw_line(kgeo::point_t p0, kgeo::point_t p1, const kgeo::draw_range_t& r);

serializer::draw_func_t draw_lines(
	const std::vector<kgeo::point_t>& ps,
	const kgeo::draw_range_t& r
);

inline serializer::draw_func_t draw_axes(
	kgeo::draw_range_t& r
)
{
	return [&](serializer& s)
	{
		s.run(draw_line({ r.left(),0 }, { r.right(),0 }, r));
		return draw_line({ 0,r.bottom() }, { 0,r.top() }, r)(s);
	};
}

serializer::draw_func_t draw_polygon(
	const kgeo::polygon_t& polygon,
	const kgeo::draw_range_t& r
);

inline serializer::draw_func_t draw_triangle(
	const kgeo::point_t& p0,
	const kgeo::point_t& p1,
	const kgeo::point_t& p2,
	const kgeo::draw_range_t& r
) {	return draw_lines({ p0,p1,p2,p0 }, r); }

serializer::draw_func_t draw_point(
	kgeo::point_t p,
	const kgeo::draw_range_t& r,
	kunit yoffset = -5_pt,
	const std::function<void(serializer&, const kgeo::point_t&)>& draw_point_f= default_draw_point_func
);

serializer::draw_func_t draw_points(
	const std::vector<kgeo::point_t>& ps,
	const kgeo::draw_range_t& r,
	kunit yoffset = 0_pt,
	const std::function<void(serializer&, const kgeo::point_t&)>& draw_point_f =
	default_draw_point_func
);


inline unary_func_t make_next_step(double step)
{
	return [step](double x) {return x + step; };
}

serializer::draw_func_t draw_func_oft(
	const unary_func_t& x_oft,
	const unary_func_t& y_oft,
	const dvec_t& ts,
	const kgeo::draw_range_t& r
);

void adaptive_make_points_from_func_oft(
	const unary_func_t& x_oft, const unary_func_t& y_oft,
	const kgeo::range_t& t_range, double mindx,
	std::vector<kgeo::point_t>& ps
);
void adaptive_make_points_from_func_oft(
	const unary_func_t& x_oft, const unary_func_t& y_oft,
	const kgeo::range_t& t_range, const serializer& s, const kgeo::draw_range_t& r,
	std::vector<kgeo::point_t>& ps
);

serializer::draw_func_t adaptive_draw_func_oft(
	const unary_func_t& x_oft,
	const unary_func_t& y_oft,
	const kgeo::range_t& t_range,
	const kgeo::draw_range_t& r
);

inline serializer::draw_func_t adaptive_draw_func_oftheta(
	const unary_func_t& r_oftheta,
	const kgeo::range_t& theta_range,
	const kgeo::draw_range_t& r
)
{
	return [&](serializer& s)
	{
		return adaptive_draw_func_oft(
			[&](double t) {return r_oftheta(t) * cos(t); },
			[&](double t) {return r_oftheta(t) * sin(t); },
			theta_range, r
		)(s);
	};
}

inline serializer::draw_func_t draw_func_ofx(
	const unary_func_t& y_ofx,
	const kgeo::draw_range_t& r,
	const kgeo::range_t& x_range = {}
) {
	static unary_func_t ft = [](double t) {return t; };
	return adaptive_draw_func_oft(
		ft, y_ofx,
		x_range.empty() ? kgeo::range_t(r.left(), r.right()) : x_range,
		r
	);
}
inline serializer::draw_func_t draw_func_ofy(
	const unary_func_t& x_ofy,
	const kgeo::draw_range_t& r,
	const kgeo::range_t& y_range = {},
	const unary_func_t& nexty = defaut_next_step_func
)
{
	static unary_func_t ft = [](double t) {return t; };
	return adaptive_draw_func_oft(
		x_ofy,ft,
		y_range.empty() ? kgeo::range_t(r.bottom(), r.top()) : y_range,
		r
	);
}

#include "kdraw_easy_def.inl"


