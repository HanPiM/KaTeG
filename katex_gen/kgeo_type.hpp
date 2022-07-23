#pragma once

#include <algorithm>

namespace kgeo
{

class range_t
{
public:

	_IC range_t()noexcept {}
	_IC range_t(double a, double b)noexcept
		: _left(std::min(a, b)), _right(std::max(a, b)) {}

	_IC double left()const noexcept { return _left; }
	_IC double right()const noexcept { return _right; }

	_IC void left(double v)noexcept { assign(v, _right); }
	_IC void right(double v)noexcept { assign(_left, v); }

	_IC void move_l(double v)noexcept { _left -= v, _right -= v; }
	_IC void move_r(double v)noexcept { _left += v, _right += v; }

	_IC double width()const noexcept { return _right - _left; }

	_IC bool contain(double v)const noexcept
	{
		return (_left <= v && v <= _right)
			|| _kfunc::deq(v, _left) || _kfunc::deq(v, _right);
	}
	_IC bool empty()const noexcept { return width() < 1e-8; }

	constexpr void assign(double L, double R)noexcept
	{
		_left = std::min(L, R);
		_right = std::max(L, R);
	}

	inline friend std::ostream& operator<<(std::ostream& o, const range_t& r)
	{
		o << '[' << r.left() << ',' << r.right() << ']';
		return o;
	}

private:
	double _left = 0, _right = 0;
};

class rect_t;

struct point_t
{
	_IC point_t() noexcept {}
	_IC point_t(double x, double y)noexcept :x(x), y(y) {}
	_IC point_t(const std::initializer_list<double> vals)
		: x(*vals.begin()), y(*(vals.begin() + 1)) {}

	_IC bool operator ==(const point_t& p)const noexcept
	{
		return _kfunc::deq(x, p.x) && _kfunc::deq(y, p.y);
	}

	_IC point_t operator +(const point_t& p)const noexcept
	{ return point_t(x + p.x, y + p.y); }

	_IC point_t operator -(const point_t& p)const noexcept
	{ return point_t(x - p.x, y - p.y); }

	_IC point_t operator *(double v)const noexcept
	{ return point_t(x * v, y * v); }

	_IC point_t operator /(double v)const { return point_t(x / v, y / v); }

	_IC const point_t& operator +=(const point_t& p)noexcept
	{
		x += p.x, y += p.y;
		return *this;
	}
	_IC const point_t& operator -=(const point_t& p)noexcept
	{
		x -= p.x, y -= p.y;
		return *this;
	}
	_IC const point_t& operator *=(double v)noexcept
	{
		x *= v, y *= v;
		return *this;
	}
	_IC const point_t& operator /=(double v)
	{
		x /= v, y /= v;
		return *this;
	}

	_IC void assign(double xx, double yy)noexcept
	{
		x = xx, y = yy;
	}
	_IC void assign(const point_t& p)noexcept
	{
		x = p.x, y = p.y;
	}

	inline friend std::ostream& operator<<(std::ostream& o, const point_t& p)
	{
		o << '(' << p.x << ',' << p.y << ')';
		return o;
	}

	double x = 0, y = 0;
};

class rect_t
{
public:

	_IC rect_t() noexcept {}
	_IC rect_t(double x1, double y1, double x2, double y2) noexcept :
		_xr(x1, x2), _yr(y1, y2) {}
	_IC rect_t(const point_t& s, const point_t& e) :
		_xr(s.x, e.x), _yr(s.y, e.y) {}

	// {x0, y0, x1, y1}
	_IC rect_t(const std::initializer_list<double> vals)
		: _xr(*vals.begin(), *(vals.begin() + 2)),
		_yr(*(vals.begin() + 1), *(vals.begin() + 3)) {}

	_IC double width()const noexcept { return _xr.width(); }
	_IC double height()const noexcept { return _yr.width(); }

	_IC const range_t& xrange()const noexcept { return _xr; }
	_IC const range_t& yrange()const noexcept { return _yr; }

	_IC void xrange(const range_t& xr)noexcept { _xr = xr; }
	_IC void yrange(const range_t& yr)noexcept { _yr = yr; }

	constexpr bool empty()const noexcept
	{
		return _xr.empty() || _yr.empty();
	}

	/*
	* ^(y+)
	* 
	* *---------epos
	* |         | ^
	* |         | h
	* |         | v
	* spos------* -  >(x+)
	* |<- w ->|
	* 
	* v(y-)
	*/

	_IC point_t spos()const noexcept { return point_t(left(), bottom()); }
	_IC point_t epos()const noexcept { return point_t(right(), top()); }

	_IC void spos(const point_t& pos)noexcept { left(pos.x), bottom(pos.y); }
	_IC void epos(const point_t& pos)noexcept { right(pos.x), top(pos.y); }

	_IC double left()const noexcept { return _xr.left(); }
	_IC double right()const noexcept { return _xr.right(); }
	_IC double bottom()const noexcept { return _yr.left(); }
	_IC double top()const noexcept { return _yr.right(); }

	_IC void left(double v)noexcept { _xr.left(v); }
	_IC void right(double v)noexcept { _xr.right(v); }
	_IC void bottom(double v)noexcept { _yr.left(v); }
	_IC void top(double v)noexcept { _yr.right(v); }

	_IC bool contain(const point_t& p)const noexcept
	{
		return _xr.contain(p.x) && _yr.contain(p.y);
	}

	_IC void move_l(double v) { _xr.move_l(v); }
	_IC void move_r(double v) { _xr.move_r(v); }
	_IC void move_u(double v) { _yr.move_r(v); }
	_IC void move_d(double v) { _yr.move_l(v); }

	_IC void move_to0() { move_l(left()), move_d(bottom()); }

	constexpr const rect_t& operator&=(const rect_t& rc)
	{
		_and(*this, rc, *this);
		return *this;
	}
	constexpr rect_t operator &(const rect_t& rc)const
	{
		rect_t tmp;
		_and(*this, rc, tmp);
		return tmp;
	}

	// p 相对于 ref_sys 的位置
	_IC friend point_t operator-(const point_t& p, const rect_t& ref_sys)
	{
		return p - ref_sys.spos();
	}
	_IC friend const point_t& operator-=(point_t& p, const rect_t& ref_sys)
	{
		p -= ref_sys.spos();
		return p;
	}

	friend std::ostream& operator<<(std::ostream& os, const rect_t& rc)
	{
		os << '{' << rc.left() << ',' << rc.bottom() << ',' << rc.right() << ',' << rc.top() << '}';
		return os;
	}

private:

	static constexpr void _and(const rect_t& rc0, const rect_t& rc1, rect_t& out)
	{
		constexpr const rect_t empty_rect = { 0,0,0,0 };

		double xl = std::max(rc0.left(), rc1.left());
		double xr = std::min(rc0.right(), rc1.right());

		double yl = std::max(rc0.bottom(), rc1.bottom());
		double yr = std::min(rc0.top(), rc1.top());

		if (_kfunc::dgeq(xl, xr) || _kfunc::dgeq(yl, yr))out = empty_rect;
		else out = rect_t(xl, yl, xr, yr);
	}

	range_t _xr, _yr;
};

class draw_range_t:public rect_t
{
public:

	_IC draw_range_t() {}
	_IC draw_range_t(const rect_t& rc, kunit zoom = 4_pt, kunit zoom_y = 0)
		:w(rc.width()* zoom), h(rc.height()* (zoom_y == 0 ? zoom : zoom_y)),rect_t(rc)
	{}
	_IC draw_range_t(kunit w, kunit h, const rect_t& rc) :w(w), h(h), rect_t(rc) { }

	_IC kunit view_width()const { return w; }
	_IC kunit view_height()const { return h; }

	constexpr kunit zoomx()const
	{
		if (w != 0 && !xrange().empty())
		{
			return std::max(1_pt, w / width());
		}
		return 1_pt;
	}
	constexpr kunit zoomy()const
	{
		if (h != 0 && !yrange().empty())
		{
			return std::max(1_pt, h / height());
		}
		return 1_pt;
	}

private:

	kunit w, h;
};

}

class serializer;

namespace kgeo
{

class polygon_t:public std::vector<point_t>
{
private:

	using _base_t = std::vector<point_t>;

public:

	polygon_t(){}
	polygon_t(const _base_t& v) :_base_t(v) {}
	polygon_t(std::initializer_list<point_t> _list) :_base_t(_list)
	{}

	polygon_t(
		const unary_func_t& x_oft,
		const unary_func_t& y_oft,
		const range_t& t_range,
		double mindx = -1,
		double step = -1
	);

	polygon_t(
		const unary_func_t& x_oft,
		const unary_func_t& y_oft,
		const range_t& t_range,
		const serializer& s, const draw_range_t& r
	);

	double min_x()const
	{
		return std::min_element(begin(), end(), _xcmp)->x;
	}
	double min_y()const
	{
		return std::min_element(begin(), end(), _ycmp)->y;
	}
	double max_x()const
	{
		return std::max_element(begin(), end(), _xcmp)->x;
	}
	double max_y()const
	{
		return std::max_element(begin(), end(), _ycmp)->y;
	}

private:

	static constexpr bool _xcmp(const point_t& a, const point_t& b) { return a.x < b.x; }
	static constexpr bool _ycmp(const point_t& a, const point_t& b) { return a.y < b.y; }
};

};
