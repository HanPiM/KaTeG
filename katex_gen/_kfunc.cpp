#include "ktype.hpp"

double _kfunc::find_root_secant(
	const unary_func_t& f, double x0, double x1, double eps
)
{
	double cur_root = x1;
	double error = eps + 1;

	double k;

	while (error > eps)
	{
		k = (f(x1) - f(x0)) / (x1 - x0);

		cur_root = cur_root - f(x1) / k;

		error = abs(f(cur_root));

		x0 = x1;
		x1 = cur_root;
	}

	return cur_root;
}

double _kfunc::find_root_binary(const unary_func_t& f, double L, double R, double eps)
{
	double cur_root = L;
	double error = eps + 1;
	double mid,fL;
	int count = 0;
	while (error > eps)
	{
		if (count > 2000)return NAN;
		fL = f(L);
		if (abs(fL) < eps) { return L; }
		else if (abs(f(R)) < eps) { return R; }
		else
		{
			mid = (L + R) / 2;
			if (fL * f(mid) < 0)R = mid;
			else L = mid;
			cur_root = L;
			count++;
		}
		error = abs(f(cur_root));
	}
	return cur_root;
}
