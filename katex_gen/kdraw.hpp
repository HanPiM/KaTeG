#pragma once

#include "serializer.hpp"

using unary_func_t = const std::function<double(double)>;
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
inline constexpr kunit empty_draw_func(serializer&) { return 0; }

serializer::draw_func_t draw_border(
	kunit w, kunit h, kunit offset = 0_pt,
	border_style style = border_style::expend_inward,
	kunit topw = 0.1_pt, kunit bottomw = 0.1_pt,
	kunit leftw = 0.1_pt, kunit rightw = 0.1_pt
);

// 由于 rule 存在误差 当格数大于 100 时会出现可见的总宽度误差
serializer::draw_func_t draw_grid(
	kgeo::rect_t grid_range,
	const kgeo::point_t& offset_pos, double dx, double dy,
	const kgeo::draw_range_t& r
);

serializer::draw_func_t draw_line(kunit w, kunit h, kunit yoffset = 0);
serializer::draw_func_t draw_line(kgeo::point_t p0, kgeo::point_t p1, const kgeo::draw_range_t& r);

serializer::draw_func_t draw_lines(
	const std::vector<kgeo::point_t>& ps,
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

// todo : 实现自适应取点绘图 
// https://yacas.readthedocs.io/en/latest/book_of_algorithms/basic.html

serializer::draw_func_t draw_func_oft(
	const unary_func_t& x_oft,
	const unary_func_t& y_oft,
	const dvec_t& ts,
	const kgeo::draw_range_t& r
);

serializer::draw_func_t draw_func_oft(
	const unary_func_t& x_oft,
	const unary_func_t& y_oft,
	const unary_func_t& nextt,
	const kgeo::range_t& t_range,
	const kgeo::draw_range_t& r
);

inline serializer::draw_func_t draw_func_ofx(
	const unary_func_t& f_ofx,
	const kgeo::draw_range_t& r,
	const kgeo::range_t& x_range = {},
	const unary_func_t& nextx = defaut_next_step_func
) {
	static unary_func_t ft = [](double t) {return t; };
	if (x_range.empty())
	{
		return draw_func_oft(
			ft, f_ofx,
			nextx,
			{ r.left(), r.right() },
			r
		);
	}
	else
	{
		return draw_func_oft(
			ft, f_ofx,
			nextx,
			x_range,
			r
		);
	}
}
inline serializer::draw_func_t draw_func_ofy(
	const unary_func_t& f_ofy,
	const kgeo::draw_range_t& r,
	const kgeo::range_t& y_range = {},
	const unary_func_t& nexty = defaut_next_step_func
)
{
	static unary_func_t ft = [](double t) {return t; };
	if (y_range.empty())
	{
		return draw_func_oft(
			f_ofy, ft,
			nexty,
			{ r.bottom(), r.top() },
			r
		);
	}
	else
	{
		return draw_func_oft(
			f_ofy, ft,
			nexty,
			y_range,
			r
		);
	}
}

#include "kdraw_easy_def.inl"


