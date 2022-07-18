#include <list>

#include "kdraw.hpp"

struct XET
{
	XET() :x(0), dx(0), ymax(0) {}
	XET(double _x, double _dx, double _ymax) :x(_x), dx(_dx), ymax(_ymax) {}

	double x;
	double dx;
	double ymax;
};
using AET = XET;
using NET = XET;

static void _fill_xline(
	serializer& s,
	double y0, double h, double sx, double ex,
	const kgeo::draw_range_t& r
)
{
	if (y0 + h < r.bottom())return;
	if (y0 < r.bottom())y0 = r.bottom();

	if (ex < sx)std::swap(ex, sx);

	if (ex >= r.right())ex = r.right();
	if (sx <= r.left())sx = r.left();

	if (_kfunc::dleq(ex, r.left()))return;
	if (_kfunc::dgeq(sx, r.right()))return;

	kunit zoom_x = r.zoomx(), zoom_y = r.zoomy();

	if ((ex - sx) * zoom_x <= 1_pt)return;

	s.kern((sx-r.left()) * zoom_x);
	s.rule((ex - sx) * zoom_x, h * zoom_y, (y0 - r.bottom()) * zoom_y);
	s.kern(-(ex-r.left()) * zoom_x);
}

serializer::draw_func_t fill_polygon(
	const kgeo::polygon_t& ps,
	const kgeo::draw_range_t& r,
	double line_step
)
{
	if (ps.size() < 3)return empty_draw_func;
	if (_kfunc::dleq(line_step, 0))line_step = 1.0 / r.zoomy().val();

	return [line_step, &ps, &r](serializer& s)
	{
		std::vector<std::list<NET>> nets;
		std::list<AET> aets;
		double miny = ps.min_y(), maxy = ps.max_y();

		auto line_id_ofy = [&miny, &line_step](double y)
		{
			return static_cast<int>(floor((y - miny) / line_step));
		};
		int line_cnt = line_id_ofy(maxy) + 1;

		nets.resize(line_cnt);

		int pre_point_idx, nxt_point_idx;
		int ps_cnt = static_cast<int>(ps.size()); // avoid stupid warning

		for (int i = 0; i < line_cnt; ++i)
		{
			for (int j = 0; j < ps.size(); ++j)
			{
				if (line_id_ofy(ps[j].y) != i)continue;
				pre_point_idx = (j - 1 + ps_cnt) % ps_cnt;
				nxt_point_idx = (j + 1 + ps_cnt) % ps_cnt;

				if (ps[pre_point_idx].y > ps[j].y)
				{
					nets[i].emplace_back(
						ps[j].x,
						(ps[pre_point_idx].x - ps[j].x) / (ps[pre_point_idx].y - ps[j].y),
						ps[pre_point_idx].y
					);
				}
				if (ps[nxt_point_idx].y > ps[j].y)
				{
					nets[i].emplace_back(
						ps[j].x,
						(ps[nxt_point_idx].x - ps[j].x) / (ps[nxt_point_idx].y - ps[j].y),
						ps[nxt_point_idx].y
					);
				}
			}
		}

		kunit zoom_x = r.zoomx(), zoom_y = r.zoomy();

		for (int i = 0; i < line_cnt; ++i)
		{
			for (auto& it : aets)
			{
				it.x += it.dx * line_step;
			}

			aets.remove_if(
				[i, &line_id_ofy](const AET& x) {return line_id_ofy(x.ymax) <= i; }
			);

			// 将 NET中的新点加入 AET 并按 x 值递增排序
			aets.splice(aets.end(), nets[i]);
			aets.sort(
				[](const AET& a, const AET& b)->bool {
					return a.x < b.x;
				}
			);

			for (auto it = aets.begin(); it != aets.end(); ++it)
			{
				auto nxt_it = it;
				++nxt_it;
				if (nxt_it == aets.end())break;

				_fill_xline(s, i * line_step + miny, line_step, it->x, nxt_it->x, r);

				it = nxt_it;
			}
		}
		return 0;
	};
}

