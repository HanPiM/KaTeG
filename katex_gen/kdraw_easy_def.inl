#pragma once

inline serializer::draw_func_t draw_ellipse(
	const kgeo::point_t& centre, double a, double b,
	const kgeo::draw_range_t& r, double alpha = 0,
	const unary_func_t& nextt= make_next_step(6.28318530718 /40)
){
	double cos_alpha = cos(alpha);
	double sin_alpha = sin(alpha);
	static constexpr kgeo::range_t t_range = { -0.1,6.28318530718 + 0.1 };

	return[&, cos_alpha, sin_alpha, a, b](serializer& s)
	{
		return draw_func_oft(
			[&, cos_alpha, sin_alpha,a,b](double t) {return centre.x + a * cos(t) * cos_alpha - b * sin(t) * sin_alpha; },
			[&, cos_alpha, sin_alpha,a,b](double t) {return centre.y + a * cos(t) * sin_alpha + b * sin(t) * cos_alpha; },
			nextt, t_range,
			r
		)(s);
	};
}

// todo : 自适应取点

inline serializer::draw_func_t fill_ellipse(
	const kgeo::point_t& centre, double a, double b,
	const kgeo::draw_range_t& r, double alpha = 0,
	double line_step=-1,
	const unary_func_t& nextt = make_next_step(6.28318530718 / 40)
) {
	double cos_alpha = cos(alpha);
	double sin_alpha = sin(alpha);
	static constexpr kgeo::range_t t_range = { -0.1,6.28318530718 + 0.1 };

	return[&, cos_alpha, sin_alpha, a, b](serializer& s)
	{
		return fill_function(
			[&, cos_alpha, sin_alpha, a, b](double t) {return centre.x + a * cos(t) * cos_alpha - b * sin(t) * sin_alpha; },
			[&, cos_alpha, sin_alpha, a, b](double t) {return centre.y + a * cos(t) * sin_alpha + b * sin(t) * cos_alpha; },
			nextt, t_range,
			r, line_step
		)(s);
	};
}