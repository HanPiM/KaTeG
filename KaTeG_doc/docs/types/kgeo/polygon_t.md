# <small>kgeo::</small>polygon_t

由顶点描述的多边形

## 基类

`public std::vector<`<small>kgeo::</small>[`point_t`](../../kgeo/point_t)`>`

## 公有函数

### 构造

`polygon_t()` 默认构造函数

`polygon_t(const std::vector<point_t>& v)` 由多个点按顺序构造

`polygon_t(std::initializer_list<point_t> _list)` 由列表中的点构造

`polygon_t(
	const `[`unary_func_t`](../../#unary_func_t)`& x_oft,
    const `[`unary_func_t`](../../#unary_func_t)`& y_oft,
    const `<small>kgeo::</small>[`range_t`](../../kgeo/range_t)`& t_range,
    double step = -1
)
`

由给出的函数确定多边形：将给定函数视为参数方程： $\begin{cases}
x=\operatorname{x\\_oft}(t)\\\\
y=\operatorname{y\\_oft}(t)
\end{cases}
$ 在 $\text{t\_range}$ 内取点构造多边形。

其中每隔 `step` 步长取一个 $t$。

如果给定步长小于等于 0，将会按照取 25 个点的方式计算步长（**非自适应取点**）。

### getter

`double min_x()const` 返回多边形最左端的横坐标

`double min_y()const` 返回多边形最下端的纵坐标

`double max_x()const` 返回多边形最右端的横坐标

`double max_y()const` 返回多边形最上端的纵坐标