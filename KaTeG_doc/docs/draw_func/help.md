# 辅助函数

## 多边形

### 三角形

特殊多边形三角形

绘制：基于 [draw_polygon](../../draw_func/draw/#draw_polygon)

```cpp
serializer::draw_func_t draw_triangle(
	const kgeo::point_t& p0,
	const kgeo::point_t& p1,
	const kgeo::point_t& p2,
	const kgeo::draw_range_t& r
);
```

填充：基于 [fill_polygon](../../draw_func/fill/#_2)

```cpp
serializer::draw_func_t fill_triangle(
	const kgeo::point_t& p0, const kgeo::point_t& p1, const kgeo::point_t& p2,
	const kgeo::draw_range_t& r, double step_line = -1
);
```

## 函数

