#pragma once

#include <iostream>
#include <iomanip>

#define _IC inline constexpr

namespace _kfunc
{
	template <typename _t>
	_IC _t abs(const _t& x) noexcept(noexcept(x > 0, -x))
	{
		return x > 0 ? x : -x;
	}
	_IC bool deq(double a, double b, double eps = 1e-7)noexcept
	{
		return abs(a - b) <= eps;
	}
	_IC bool dleq(double a, double b, double eps = 1e-7)noexcept
	{
		return a - b <= eps;
	}
	_IC bool dgeq(double a, double b, double eps = 1e-7)noexcept
	{
		return dleq(b, a, eps);
	}
};

class kunit
{
public:

	_IC kunit()noexcept :_val(0) {}
	_IC kunit(double x)noexcept :_val(x) {}
	_IC kunit(const kunit& x) : _val(x._val) {}
	~kunit() = default;

	_IC bool operator<(kunit x)const { return _val < x._val; }
	_IC bool operator>(kunit x)const { return _val > x._val; }
	_IC bool operator<=(kunit x)const { return _val <= x._val; }
	_IC bool operator>=(kunit x)const { return _val >= x._val; }
	_IC bool operator==(kunit x)const { return _eq(_val, x._val); }
	_IC bool operator!=(kunit x)const { return !_eq(_val, x._val); }
	_IC const kunit& operator+=(const kunit& x) { _val += x._val; return x; }
	_IC const kunit& operator-=(const kunit& x) { _val -= x._val; return x; }
	_IC const kunit& operator*=(double x) { _val *= x; return *this; }
	_IC const kunit& operator/=(double x) { _val /= x; return *this; }

	_IC kunit operator+(kunit x)const { return kunit(_val + x._val); }
	_IC kunit operator-(kunit x)const { return kunit(_val - x._val); }
	_IC kunit operator*(double x)const { return kunit(_val * x); }
	_IC kunit operator/(double x)const { return kunit(_val / x); }

	_IC friend kunit operator*(double x, kunit val) { return val * x; }

	_IC const kunit& operator=(const kunit& x) { _val = x._val; return x; }

	_IC kunit operator-() const { return kunit(-_val); }

	_IC double val()const { return _val; }
	_IC explicit operator double() { return _val; }

	friend std::ostream& operator << (std::ostream& os, const kunit& x)
	{
		int decimal_part = int(x._val * 1000) % 1000;
		int decimal_part_wide = 3;

		if (decimal_part == 0)decimal_part_wide = 0;
		else if (decimal_part % 100 == 0)decimal_part_wide = 1;
		else if (decimal_part % 10 == 0)decimal_part_wide = 2;

		auto old_wide = os.precision();
		os << std::fixed << std::setprecision(decimal_part_wide);

		if (x != 0 && x.abs() < 0.01)os << x._val * 65536 << "sp";
		else os << x._val << "pt";

		os.precision(old_wide);
		return os;
	}

	_IC kunit abs()const { return kunit(_kfunc::abs(_val)); }

private:

	static _IC double _to_sp(double x) { return x / 65536.0; }

	//_IC bool _divisible(double x, double eps = 1)const
	//{
	//	double tmp = _abs(_val);
	//	return tmp - int(tmp / x) * x <= eps;
	//}
	//_IC double _as_unit(double base)const
	//{
	//	return _val / base;
	//}

	static _IC bool _eq(double a, double b)
	{
		return _kfunc::deq(a, b, _to_sp(0.01));
	}

	double _val;
};


#define _MAKE(name,expr)\
inline constexpr kunit name(double x){return kunit(expr);}\
inline constexpr kunit operator ""##_##name (unsigned long long _raw)\
{return name(double(_raw));}\
inline constexpr kunit operator ""##_##name (long double _raw)\
{return name(double(_raw));}

_MAKE(pt, x) _MAKE(pc, 12 * x) _MAKE(dd, 1238.0 * x / 1157.0)
_MAKE(cc, 14856.0 * x / 1157.0) _MAKE(nd, 685.0 * x / 642.0)
_MAKE(nc, 1370.0 * x / 107.0) _MAKE(sp, x / 65536.0)
// 1pt = 1/72.27 in => 1in = 72.27pt
_MAKE(in, 72.27 * x)
// 1bp = 1/70 in => 72.27/70 pt
_MAKE(bp, 72.27 * x / 70.0)
// 1px = 1bp ???
_MAKE(px, 72.27 * x / 70.0)
// 1in = 2.54cm, 1in = 72.27pt => 1cm = 7227/254 pt
_MAKE(cm, 7227 * x / 254.0)
// 1mm = 1/100 cm = 7227/25400 pt
_MAKE(mm, 7227 * x / 25400.0)

#undef _MAKE

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

	_IC double size()const noexcept { return _right - _left; }

	_IC bool contain(double v)const noexcept
	{
		return (_left <= v && v <= _right)
			|| _kfunc::deq(v, _left) || _kfunc::deq(v, _right);
	}
	_IC bool empty()const noexcept { return size() < 1e-8; }

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

	//_IC bool in_rect(const rect_t& r)const noexcept
	//{
	//	return r.contain(*this);
	//}
	//constexpr void cut_into(const rect_t& r)noexcept
	//{
	//	if (x < r.left())x = r.left();
	//	if (x > r.right())x = r.right();
	//	if (y < r.bottom())y = r.bottom();
	//	if (y > r.top())y = r.top();
	//}
	//constexpr point_t get_cut_into(const rect_t& r)const noexcept
	//{
	//	point_t res = *this;
	//	res.cut_into(r);
	//	return res;
	//}

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

	_IC double width()const noexcept { return _xr.size(); }
	_IC double height()const noexcept { return _yr.size(); }

	_IC const range_t& xrange()const noexcept { return _xr; }
	_IC const range_t& yrange()const noexcept { return _yr; }

	_IC void xrange(const range_t& xr)noexcept { _xr = xr; }
	_IC void yrange(const range_t& yr)noexcept { _yr = yr; }

	constexpr bool empty()noexcept
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

enum class kitem_size
{
	normal,

	tiny,
	script,
	footnote,
	small,
	large,
	Large,
	LARGE,
	huge,
	Huge,
};

enum class kalign_style
{
	left,
	center,
	right
};

struct kcolor
{
	enum :unsigned int
	{
		transparent = 0xffffffffu
	};
};

std::string kcolor_to_string(unsigned int col);

struct karg_tok
{
	karg_tok(const std::string& s) :str(s) {};
	const std::string& str;
};

#undef _IC
