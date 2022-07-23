# 绘制类函数

## kdraw.hpp

### 边框（draw_border）

### 线段（draw_line）

`serializer::draw_func_t draw_line(kgeo::point_t p0, kgeo::point_t p1, const kgeo::draw_range_t& r);`

绘制无向线段 P0P1，如有超出部分会自行裁切线段。

<small>**最小精确**：缩放后横向宽度不能小于下述 “最小精度”</small>

| item_size | 最小精确(pt) | item_size | 最小精确(pt) |
| -------- | ---- | -------- | ---- |
| normal   | 4    | large    | 4.8  |
| tiny     | 2    | Large    | 5.6  |
| script   | 2.8  | LARGE    | 7  |
| footnote | 3.2  | huge     | 8    |
| small    | 3.6  | Huge     | 10    |

### 连点成线（draw_lines）

```cpp
serializer::draw_func_t draw_lines(
	const std::vector<kgeo::point_t>& ps,
	const kgeo::draw_range_t& r
);
```

按顺序依次绘制相邻的两个点。

**最小精度** 前置：[draw_line](./#draw_line)

### 网格（draw_grid）

```cpp
serializer::draw_func_t draw_grid(
	kgeo::rect_t grid_range,
	const kgeo::point_t& offset_pos, double dx, double dy,
	const kgeo::draw_range_t& r
);
```

- `grid_range` 网格范围
- `offset_pos` 网格相对于 `grid_range` 左下角的偏移
- `dx`，`dy` 网格的长和宽
- `r` 绘制范围

??? example  end "样例"
 	$\rule{135pt}{1pt}\kern{-135pt}\rule[30pt]{135pt}{1pt}\kern{-135pt}\rule[60pt]{135pt}{1pt}\kern{-135pt}\rule[90pt]{135pt}{1pt}\kern{-135pt}\rule{1pt}{105pt}\kern{29pt}\rule{1pt}{105pt}\kern{29pt}\rule{1pt}{105pt}\kern{29pt}\rule{1pt}{105pt}\kern{29pt}\rule{1pt}{105pt}\kern{-121pt}\color{#f00}\rule[7.5pt]{60pt}{1pt}\kern{-60pt}\rule[37.5pt]{60pt}{1pt}\kern{-60pt}\rule[67.5pt]{60pt}{1pt}\kern{-45pt}\rule{1pt}{90pt}\kern{29pt}\rule{1pt}{90pt}\clap{\bf offset:(1/2,1/4)}\kern{-46pt}\color{lightblue}\kern{60pt}\rule[15pt]{60pt}{1pt}\kern{-60pt}\rule[45pt]{60pt}{1pt}\kern{-60pt}\rule[75pt]{60pt}{1pt}\kern{-52.5pt}\rule{1pt}{90pt}\kern{29pt}\rule{1pt}{90pt}\text{\bf offset:(1/4,1/2)}$
	```
	s.run(draw_grid(
		{0,0,10,10},
		{ 0,0 }, 1, 1,
		r
	)); // 黑色网格，offset 为 (0,0)
	s.run(draw_grid(
		{ 0,0,2,3 },
		{ 0.5,0.25 }, 1, 1,
		r
	)); // 红色网格，offset 为 (0.5,0.25)
	s.run(draw_grid(
		{ 2,0,4,3 },
		{ 0.25,0.5 }, 1, 1,
		r
	)); // 蓝色网格，offset 为 (0.25,0.5)
	```

### 多边形（draw_polygon）

```cpp
serializer::draw_func_t draw_polygon(
	const kgeo::polygon_t& polygon,
	const kgeo::draw_range_t& r
);
```

每点与下一点之间绘制一条线段，然后绘制第一点到第二点的连线。

### 函数（draw_func_oft）

```cpp
serializer::draw_func_t draw_func_oft(
	const unary_func_t& x_oft,
	const unary_func_t& y_oft,
	const dvec_t& ts,
	const kgeo::draw_range_t& r
);
```

绘制参数方程 $\begin{cases}
x=\operatorname{x\_oft}(t)\\
y=\operatorname{y\_oft}(t)
\end{cases}$， $t$ 从 `ts` 中取，得到对应的多个点，然后调用 [draw_lines](./#draw_lines) 绘制。