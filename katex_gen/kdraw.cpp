#include "kdraw.hpp"
#include "s_predef.h"

void default_draw_point_func(serializer& s, const kgeo::point_t& p)
{
    if (s.mode() & serializer_mode::no_def)
    {
        s.begin_x_overlapping_context(kalign_style::center);
        s.add_char('*');
        s.end_context();
    }
    else
    {
        if (!s.has_predef(PREDEF_DEFAULT_POINT))
        {
            BEGIGN_PREDEF(s, PREDEF_DEFAULT_POINT);
            default_draw_point_func(tmps, p);
            END_PREDEF(s, PREDEF_DEFAULT_POINT);
        }
        s.call_predef(PREDEF_DEFAULT_POINT);
    }
}

serializer::draw_func_t draw_border(
    kunit w, kunit h, kunit yoffset, border_style style,
    kunit topw, kunit bottomw, kunit leftw, kunit rightw
) {
    kunit raw_w = w;
    if (bottomw.abs() <= 0.5_pt)bottomw = 0;
    if (topw.abs() <= 0.5_pt)topw = 0;

    if (leftw.abs() <= 0.5_pt)leftw = 0.1_pt;
    if (rightw.abs() <= 0.5_pt)rightw = 0.1_pt;

    kunit x_loffset = 0_pt, x_roffset = 0_pt;

    if (style != border_style::expend_inward)
    {
        switch (style)
        {
        case border_style::expend_middle:
            x_loffset = -leftw / 2;
            x_roffset = -rightw / 2;
            yoffset -= bottomw / 2;
            w += (leftw + rightw) / 2;
            h += (topw + bottomw) / 2;
            break;
        case border_style::expend_outward:
            x_loffset = -leftw;
            x_roffset = -rightw;
            yoffset -= bottomw;
            w += leftw + rightw;
            h += topw + bottomw;
            break;
        default:
            break;
        }
    }

    return [raw_w,x_loffset, x_roffset, yoffset,
        w, h, topw, leftw, bottomw, rightw](serializer& s)
    {
        s.kern(x_loffset);

        s.rule(leftw, h, yoffset); s.kern(-leftw);
        s.rule(w, bottomw, yoffset); s.kern(-w);
        s.rule(w, topw, h - topw + yoffset); s.kern(-rightw);
        s.rule(rightw, h, yoffset);

        s.kern(x_roffset);
        return raw_w;
    };
}

serializer::draw_func_t draw_grid(
    kgeo::rect_t grid_range,
    const kgeo::point_t& offset_pos, double dx, double dy,
    const kgeo::draw_range_t& r
)
{
    if (r.view_width() == 0 && r.view_height() == 0)return empty_draw_func;

    grid_range &= r;

    if (grid_range.xrange().empty() && grid_range.yrange().empty())
        return empty_draw_func;

    kgeo::point_t spos = offset_pos;

    spos.x = spos.x - floor(spos.x/dx)*dx;
    spos.y = spos.y - floor(spos.y/dy)*dy;
    
    // std::cout << "grid_r " << grid_range << " offset " << spos << '\n';

    if (grid_range.xrange().empty())grid_range.xrange(r.xrange());
    if (grid_range.yrange().empty())grid_range.yrange(r.yrange());

    return [dx, dy, spos, grid_range, &r](serializer& s)
    {
        kunit zoom_x = r.zoomx(), zoom_y = r.zoomy();
        kunit w = grid_range.width() * zoom_x, h = grid_range.height() * zoom_y;

        double sx = grid_range.left(), sy = grid_range.bottom(),
            ex = grid_range.right(),
            ey = grid_range.top();

        sx += spos.x, ex -= spos.x;
        sy += spos.y, ey -= spos.y;

        //std::cout << "sx " << sx << " ex " << ex << " sy " << sy << " ey " << ey << '\n';

        double yoffset = grid_range.bottom() - r.bottom();
        double xoffset = grid_range.left() - r.left();

        s.kern(xoffset * zoom_x);
        for (double y = sy; y <= ey; y += dy)
        {
            s.rule(w, 0_pt, zoom_y * (y - r.bottom()));
            s.kern(-w);
        }
        s.kern(spos.x * zoom_x);
        double x;
        for (x = sx; x <= ex; x += dx)
        {
            if (!_kfunc::deq(x, sx))s.kern(zoom_x * dx);
            s.rule(0.5_pt, h, yoffset * zoom_y);
            s.kern(-0.5_pt);
        }
        return w - zoom_x * (x - ex) + xoffset * zoom_x;
    };
}

serializer::draw_func_t draw_line(kunit w, kunit h, kunit yoffset)
{
    // of[text_size::xxx]
    static const constexpr kunit offset_of[] =
    {
        2_pt,

        1_pt,1.4_pt,1.6_pt,1.8_pt,
        2.4_pt,2.8_pt,3.5_pt,
        4_pt, 5_pt
    };

    /* debug code:
    $
    \def\flag{\mathrlap{\rule[-5pt]{0.5pt}{30pt}}}
    \def\offset{5pt}
    \Huge
    a\flag
    \kern{\offset}
    \flag
    \cancel{
        \color{red}\rule{40pt}{20pt}
        \kern{-\offset}\kern{-\offset}
    }\kern{\offset}
    \flag
    $
    */

    if (h.abs() <= 0.5_pt)
    {
        return [w, yoffset](serializer& s) {s.rule(w, 0, yoffset); return w; };
    }
    if (w.abs() <= 1_pt)
    {
        return [h, yoffset](serializer& s) {s.rule(1_pt, h, yoffset); return 1_pt; };
    }

    if (w < 0)w = -w, h = -h;
    bool bflag = h < 0;
    if (bflag)h = -h;

    return [w, h, bflag, yoffset](serializer& s)
    {
        auto yraise = yoffset + (bflag ? -h : 0);
        auto item_size = static_cast<uint32_t>(s.item_size());

        // rule(w,h) Îó²î¹ý´ó ¾Ü¾ø»æÖÆ
        if (_kfunc::dleq(w.abs().val(), 2 * offset_of[item_size].val() + 1.5))
        {
            if (w == 0)
            {
                s.rule(1_pt, h, yoffset);
                return 1_pt;
            }

            double k = h.val() / w.val();
            if (bflag)k = -k;

            double dot_siz = 0.8;

            //return 0_pt;

            kunit v = 0;
            for (; _kfunc::dleq(v.val(), w.val()); v += dot_siz)
            {
                s.rule(dot_siz, dot_siz, yoffset + v * k);
            }

            return kunit(v);
        }
        //std::cout << "w " << w.abs() << " minoffset " << 2 * offset_of[item_size].val() << '\n';

        s.kern(offset_of[item_size]);

        if (yraise != 0)
        {
            s.begin_raisemath_context(yraise);
        }

        s.add_char('\\');
        
        if(bflag)s.add_char('b');

        s.add_str("cancel{\\phantom");
        s.begin_empty_context();

        {
            s.rule(w - 2 * offset_of[item_size], h);
        }
        s.end_context();
        s.add_char('}');

        if (s.mode() & serializer_mode::for_discuss)
        {
            s.kern(offset_of[item_size] * 2);
        }
        else s.kern(offset_of[item_size]);

        if (yoffset != 0)
        {
            s.end_context();
        }

        return w;
    };
}

serializer::draw_func_t draw_line(kgeo::point_t p0, kgeo::point_t p1, const kgeo::draw_range_t& r)
{
    kunit xoffset, yoffset, w, h;
    kunit zoom_x = r.zoomx(), zoom_y = r.zoomy();

    if (_kfunc::deq(p0.x, p1.x))
    {
        if(!r.xrange().contain(p0.x))return empty_draw_func;
        if (p0.y > p1.y)std::swap(p0, p1);

        double yl = std::max(r.bottom(), p0.y);
        double yr = std::min(r.top(), p1.y);

        if (_kfunc::dgeq(yl, yr))return empty_draw_func;

        yl -= r.bottom(), yr -= r.bottom();
        xoffset = (p0.x - r.left()) * zoom_x;
        yoffset = yl * zoom_y;
        return[zoom_x, zoom_y, yl, yr, xoffset, yoffset](serializer& s)
        {
            s.kern(xoffset);
            return xoffset + draw_line(0, (yr - yl) * zoom_y, yoffset)(s);
        };
    }

    double k = (p1.y - p0.y) / (p1.x - p0.x);
    double b = p0.y - k * p0.x;

    if (_kfunc::deq(k, 0))
    {
        if (!r.yrange().contain(p0.y))return empty_draw_func;

        if (p0.x > p1.x)std::swap(p0, p1);
        double xl = std::max(r.left(), p0.x);
        double xr = std::min(r.right(), p1.x);

        if (_kfunc::dgeq(xl, xr))return empty_draw_func;

        xl -= r.left(), xr -= r.left();
        yoffset = (p0.y - r.bottom()) * zoom_y;

        return [zoom_x, xl, xr, yoffset](serializer& s)
        {
            s.kern(xl * zoom_x);
            return xl * zoom_x + draw_line((xr - xl) * zoom_x, 0, yoffset)(s);
        };
    }

    kgeo::rect_t rc(p0, p1);

    rc &= r;
    
    if (rc.empty())return empty_draw_func;
    
    if (r.contain(p0) && r.contain(p1)) {}
    else
    {
        kgeo::point_t intersections[] = {
            {r.left(),r.left() * k + b},
            {r.right(),r.right() * k + b},

            {(r.top() - b) / k,r.top()},
            {(r.bottom() - b) / k,r.bottom()},

            p0,p1
        };

        int ok_ps_cnt = 0;
        kgeo::point_t ok_ps[6];
        for (auto& it : intersections)
        {
            if (rc.contain(it))
            {
                bool appeared = false;
                for (auto& ok_p : ok_ps)
                {
                    if (_kfunc::deq(ok_p.x, it.x))
                    {
                        appeared = true;
                        break;
                    }
                }
                if (appeared)continue;
                ok_ps[ok_ps_cnt++] = it;
            }
        }
        if (ok_ps_cnt > 2)
        {
            throw std::logic_error("not reach(ok_ps_cnt>2)");
        }
        if (ok_ps_cnt < 2)
        {
            throw std::logic_error("not reach(ok_ps_cnt<2)");
        }

        p0 = ok_ps[0], p1 = ok_ps[1];
    }

    if (p0.x > p1.x)std::swap(p0, p1);

    p0 -= r, p1 -= r;

    xoffset = p0.x * r.zoomx(), yoffset = p0.y * r.zoomy();
    w = (p1.x - p0.x) * zoom_x, h = (p1.y - p0.y) * zoom_y;

    //std::cout << p0 << "->" << p1 << '\n';

    return [w,h,xoffset,yoffset](serializer& s)
    {
        s.kern(xoffset);
        return xoffset + draw_line(w, h, yoffset)(s);
    };
}

serializer::draw_func_t draw_lines(
    const std::vector<kgeo::point_t>& ps, const kgeo::draw_range_t& r
)
{
    if (ps.size() < 2)return empty_draw_func;
    return [&](serializer& s) {
        auto last = ps.begin(), cur = ps.begin() + 1;
        kunit res = 0;
        for (; cur != ps.end(); ++cur)
        {
            s.kern(-draw_line(*last, *cur, r)(s));
            last = cur;
        }
        return res;
    };
}

serializer::draw_func_t draw_point(
    kgeo::point_t p, const kgeo::draw_range_t& r,
    kunit yoffset,
    const std::function<void(serializer&, const kgeo::point_t&)>& draw_point_f
){
    kunit zoom_x = r.zoomx(), zoom_y = r.zoomy();

    if (!r.xrange().empty() && !r.xrange().contain(p.x))
        return empty_draw_func;
    if (!r.yrange().empty() && !r.yrange().contain(p.y))
        return empty_draw_func;

    p -= r;

    return [&,zoom_x,zoom_y](serializer& s)
    {
        s.kern(zoom_x * p.x);
        auto tmp = zoom_y * p.y + yoffset;
        if (tmp != 0)s.begin_raisebox_context(tmp);
        draw_point_f(s, p);
        s.end_context();
        return zoom_x * p.x;
    };
}

serializer::draw_func_t draw_points(
    const std::vector<kgeo::point_t>& ps, const kgeo::draw_range_t& r,
    kunit yoffset,
    const std::function<void(serializer&, const kgeo::point_t&)>& draw_point_f
)
{
    if (ps.empty())return empty_draw_func;
    if (ps.size() == 1)
    {
        return draw_point(ps[0], r, yoffset, draw_point_f);
    }

    return [&](serializer& s) {
        auto it = ps.begin();
        kunit w = draw_point(*it, r, yoffset, draw_point_f)(s);
        for (++it; it != ps.end(); ++it)
        {
            s.kern(-w);
            w = draw_point(*it, r, yoffset, draw_point_f)(s);
        }
        return w;
    };
}

serializer::draw_func_t draw_func_oft(
    const unary_func_t& x_oft, const unary_func_t& y_oft,
    const dvec_t& ts, const kgeo::draw_range_t& r
)
{
    return [&](serializer& s)
    {
        std::vector<kgeo::point_t> ps;
        for (auto t : ts)
        {
            ps.emplace_back(x_oft(t), y_oft(t));
        }
        return draw_lines(ps, r)(s);
    };
}

serializer::draw_func_t draw_func_oft(
    const unary_func_t& x_oft, const unary_func_t& y_oft,
    const unary_func_t& nextt, const kgeo::range_t& t_range,
    const kgeo::draw_range_t& r
){
    return [&](serializer& s)
    {
        std::vector<kgeo::point_t> ps;
        for (double t = t_range.left(); _kfunc::dleq(t, t_range.right()); t = nextt(t))
        {
            ps.push_back({ x_oft(t),y_oft(t) });
        }
        return draw_lines(ps, r)(s);
    };
}

