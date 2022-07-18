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

using unary_func_t = const std::function<double(double)>;

#include "kgeo_type.hpp"

#undef _IC

