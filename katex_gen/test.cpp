#include "kdraw.hpp"
#include <iostream>

constexpr kgeo::range_t func()
{
	
	kgeo::range_t r;
	r.assign(10, 20);
	r.right(1.14);

	kgeo::point_t p(9,2);
	p *= 3;

	r.left(r.left() + p.x);

	return r;
}

int main()
{

	serializer s;

	std::cout << std::setw(0);

	s.mode(s.mode()
		//| serializer_mode::inherit_item_size
		//| serializer_mode::for_discuss
		| serializer_mode::no_def
	);

	//s.color(0xcccccc);

	auto r = kgeo::draw_range_t({ -6.5,-4.5,12.5,4.5 }, 20_pt);
	
	s.item_size(kitem_size::tiny);

	s.color(0xeeeeee);

	//s.run(fill_polygon(
	//	{
	//		{0,0},{1,-1},{2,1},{1,1},{2,-1},{1.5,0.5}
	//		/*{2,4},{2.667,2},{3.5,2},
	//		{4,3},{4.333 + 4.0 / 15.0,4+4.0 / 5.0},
	//		{4.25,5.5}*/
	//	},
	//	r
	//));

	s.run(fill_ellipse({ -3,0 }, 2.94, 1.5, r, 2.55));
	s.color(0);
	s.run(draw_ellipse({ -3,0 }, 2.94, 1.5, r, 2.55));

	/*

	s.run(draw_line({ 1,4 }, { 2,2 }, r));
	s.run(draw_line({ 1,4 }, { 2,6 }, r));
	
	s.run(draw_line({ 2,4 }, { 3,1 }, r));
	s.run(draw_line({ 2,4 }, { 5,6 }, r));
	
	s.run(draw_line({ 3,1 }, { 4,3 }, r));
	
	s.run(draw_line({ 4,3 }, { 5,6 }, r));
	s.run(draw_line({ 4,2 }, { 5,4 }, r));
	s.run(draw_line({ 4,6 }, { 5,4 }, r));
	
	s.run(draw_line({ 2,2 }, { 4,2 }, r));
	s.run(draw_line({ 2,6 }, { 4,6 }, r));
	//*/

	//s.run(draw_func_ofx(
	//	[](double x) {return x * exp(x); },
	//	r, {},
	//	make_next_step(0.2)
	//));

	double a = 5;

	s.run(draw_func_oft(
		[a](double t) {return a * pow(cos(t), 3); },
		[a](double t) {return a * pow(sin(t), 3); },
		make_next_step(0.2),
		{ 0,2 * acos(-1) },
		r
	));
	//s.run(draw_func_oft(
	//	[](double t) {return 16 * pow(sin(t), 3)/6.0; },
	//	[](double t) {return (13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t)) / 6.0; },
	//	make_next_step(0.1),
	//	{ 0,2 * acos(-1) },
	//	r
	//));

	s.run(draw_func_oft(
		[a](double t) {return 3 * a * t / (1 + t * t * t); },
		[a](double t) {return 3 * a * t * t / (1 + t * t * t); },
		make_next_step(0.1),
		{ 0,6.28 },
		r
	));

	s.run(draw_func_oft(
		[a](double t) {return a * pow(cos(t), 3); },
		[a](double t) {return a * pow(sin(t), 3); },
		make_next_step(0.2),
		{ 0,2 * acos(-1) },
		r
	));

	//s.run(draw_points(
	//	{
	//		{1,4},
	//		{2,2},{2,4},{2,6},
	//		{3,1},
	//		{4,2},{4,3},{4,6},
	//		{5,4},{5,6}
	//	}, r
	//)
	//);
	s.color(0xcccccc);

	s.run(draw_grid(
		{0,-4.5,12.5,4.5},
		{ 0,0.5 }, 1, 1,
		r
	));

	s.color(0);

	s.run(draw_line({ -6.5,0 }, { 12.5,0 }, r));
	s.run(draw_line({ 0,-6.5 }, { 0,6.5 }, r));

	s.end_context();

	s.end_all_context();
	s.output_to(std::cout);


	return 0;
}
