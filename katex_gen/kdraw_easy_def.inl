#pragma once

// todo : 自适应取点

inline kgeo::polygon_t make_ellipse(
	const kgeo::point_t& centre, double a, double b, double alpha,
	const kgeo::range_t& t_range = { -0.1,6.28318530718+0.1 }, double t_step = -1
) {
	double cos_alpha = cos(alpha);
	double sin_alpha = sin(alpha);

	return kgeo::polygon_t(
		[&, cos_alpha, sin_alpha, a, b](double t) {return centre.x + a * cos(t) * cos_alpha - b * sin(t) * sin_alpha; },
		[&, cos_alpha, sin_alpha, a, b](double t) {return centre.y + a * cos(t) * sin_alpha + b * sin(t) * cos_alpha; },
		t_range, t_step
	);
}