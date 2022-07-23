#include "kdraw.hpp"
#include <iostream>


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

	auto r = kgeo::draw_range_t({ -2.5,-2.5,2.5,2.5 }, 30_pt);
	
	//s.item_size(kitem_size::tiny);

	std::cout << std::fixed;

	auto g = kgeo::polygon_t(
		[](double t) {return 2 * sin(4 * t) * cos(t); },
		[](double t) {return 2 * sin(4 * t) * sin(t); },
		{0,2*3.1415926},
		s,r
	);

	puts("dots:");
	for (auto& p : g)
	{
		std::cout << p << ',';
	}
	puts("");

	s.color(0xeeeeee);
	s.run(fill_polygon(g, r));

	s.color(0xaaaaaa);
	double a = 1;
	//s.run(adaptive_draw_func_oft(
	//	[a](double t) {return 3 * a * t / (1 + t * t * t); },
	//	[a](double t) {return 3 * a * t * t / (1 + t * t * t); },
	//	{-1,10},r
	//));
	s.color(0xff0000);
	s.run(draw_polygon(g, r));
	//s.run(adaptive_draw_func_oftheta(
	//	[](double theta)
	//	{
	//		return 2*sin(2 * theta);
	//	},{0,6.5},r
	//));
	s.color(0xcccccc);
	//s.run(draw_grid({ -4,-4,4,4 }, { 0.5,0.5 }, 1, 1, r));
	s.run(draw_axes(r));

	s.end_all_context();
	s.output_to(std::cout);


	return 0;
}
