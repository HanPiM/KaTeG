# 填充类绘制函数

## kdraw_fill.hpp

### 多边形（fill_polygon）

```cpp
serializer::draw_func_t fill_polygon(
	const kgeo::polygon_t& polygon,
	const kgeo::draw_range_t& r,
	double line_step = -1
);
```
纵向扫描多边形，横向填充。


- `polygon` 要填充的多边形
- `r` 绘制范围
- `line_step` 填充过程中扫描线移动的步长（相当于精度，该值越小绘制效果越好）

---

内部实现：多边形扫描线填充算法

**最小精确** 前置：[serializer::rule](../../types/serializer/#katex)

### 曲线