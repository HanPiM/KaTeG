#include "kdraw.hpp"

extern const kunit line_offset_of[10];

serializer::draw_func_t adaptive_draw_func_oft(
    const unary_func_t& x_oft, const unary_func_t& y_oft,
    const kgeo::range_t& t_range, const kgeo::draw_range_t& r
)
{
    return [&](serializer& s)
    {
        std::vector<kgeo::point_t> ps;
        adaptive_make_points_from_func_oft(
            x_oft, y_oft, t_range,
            s,r,
            ps
        );
        return draw_lines(ps, r)(s);
    };
}

void adaptive_make_points_from_func_oft(
    const unary_func_t& x_oft, const unary_func_t& y_oft,
    const kgeo::range_t& t_range, const serializer& s, const kgeo::draw_range_t& r,
    std::vector<kgeo::point_t>& ps
)
{
    adaptive_make_points_from_func_oft(
        x_oft, y_oft, t_range,
        2 * line_offset_of[static_cast<int>(s.item_size())].val() / r.zoomx().val(),
        ps
    );
}

void adaptive_make_points_from_func_oft(
    const unary_func_t& x_oft, const unary_func_t& y_oft,
    const kgeo::range_t& t_range, double mindx,
    std::vector<kgeo::point_t>& ps
) {
    using _kfunc::isbad, _kfunc::deq;

    constexpr double dx_eps = 0.1;
    double x, y, t, tmp;
    double last_apply_t;
    const double dt = t_range.width() / 100.0;
    mindx += dx_eps;

    t = t_range.left();

    while (true)
    {
        x = x_oft(t), y = y_oft(t);
        if (isinf(x) || isnan(x) || isinf(y) || isnan(y))
        {
            t += dt;
            continue;
        }
        break;
    }
    last_apply_t = t;
    ps.emplace_back(x, y);
    while (islessequal(t, t_range.right()))
    {
        ///*
        tmp = abs(x_oft(t) - x);
        if (tmp > mindx)
        {
            x = x_oft(t), y = y_oft(t);
            ps.emplace_back(x, y);
            t += dt;
            continue;
        }
        else
        {
            double x_0, x_1, x_2;
            double y_0, y_1, y_2;
            double k1, k2;
            x_0 = x_oft(t - dt), x_1 = x_oft(t), x_2 = x_oft(t + dt);
            y_0 = y_oft(t - dt), y_1 = y_oft(t), y_2 = y_oft(t + dt);
            
            k1 = (y_2 - y_1) / (x_2 - x_1);
            k2 = (y_1 - y_0) / (x_1 - x_0);
            
            if (k1 * k2 < 0)
            {
                ps.emplace_back(x_1, y_1);
                ps.emplace_back(x_2, y_2);
                t += dt;
            }
        }
        t += dt;
        continue;
        //*/

        /*
        * F(t) = |x_oft(t) - x|
        * 使 F(t) = 0
        */

        double t_0 = _kfunc::find_root_binary(
            [&x_oft, x](double t)
            {
                return abs(x_oft(t) - x);
            }, t + dt, t + 2 * dt

        );
        // F(t) = mindx
        double t_mindx = _kfunc::find_root_binary(
            [&x_oft, x, mindx](double t)
            {
                return abs(x_oft(t) - x) - mindx;
            }, t, t + dt
        );

        double v;

        if (isbad(t_0))
        {
            if (isbad(t_mindx))
            {
                t += dt;
                continue;
            }
            else v = t_mindx;
        }
        else
        {
            if (isbad(t_mindx))v = t_0;
            else
            {
                v = std::min(t_0, t_mindx);
            }
        }

        //printf("t: %lf v: %lf\n", lastt, v);
        ///*

        // 寻找横向"拐点"
        const double da = (v - last_apply_t) / 50.0;
        if (isgreater(da, 0))
        {
            double s = last_apply_t - 1e9;
            double x_0, x_1, x_2;
            double y_0, y_1, y_2;
            double k1, k2;

            bool has_inflection = false;

            x_0 = x_oft(last_apply_t), x_2 = x_oft(v);
            y_0 = y_oft(last_apply_t), y_2 = y_oft(v);

            for (double a = last_apply_t + da; a + da <= v; a += da)
            {
                x_1 = x_oft(a), y_1 = y_oft(a);
                k1 = (y_2 - y_1) / (x_2 - x_1);
                k2 = (y_1 - y_0) / (x_1 - x_0);

                double dtheta = abs(k1 - k2 / (1 + k1 * k2));

                if (dtheta >= 1)
                {
                    x_0 = x_1, y_0 = y_1;
                    has_inflection = true;
                }
                //if (k1 * k2 < 0) // 当前为转折点
                //{
                //    x_0 = x_1, y_0 = y_1; // 尝试后移，如仍存在则该点非最边缘转折点
                //    has_inflection = true;
                //}
            }
            if (has_inflection)
            {
                ps.emplace_back(x_0, y_0);
                std::cout << ps.back() << ',';
            }
        }
        //*/

        last_apply_t = t = v;
        //std::cout <<t<< ',';
        x = x_oft(t);
        y = y_oft(t);
        ps.emplace_back(x, y);
        std::cout << ps.back() << ',';
    }
}
